// huffman_encode.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <cmath>


using namespace std;

// A Tree node
struct Node
{
	char ch;
	int freq;
	Node *left, *right;
};

// Function to allocate a new tree node
Node* getNode(char ch, int freq, Node* left, Node* right)
{
	Node* node = new Node();

	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;

	return node;
}

// Comparison object to be used to order the heap
struct comp
{
	bool operator()(Node* l, Node* r)
	{
		// highest priority item has lowest frequency
		return l->freq > r->freq;
	}
};

// traverse the Huffman Tree and store Huffman Codes
// in a map.
void encode(Node* root, string str,
	unordered_map<char, string> &huffmanCode)
{
	if (root == nullptr)
		return;

	// found a leaf node
	if (!root->left && !root->right) {
		huffmanCode[root->ch] = str;
	}

	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}

// Builds Huffman Tree and decode given input text
unordered_map<char, string> buildHuffmanTree(string text)
{
	// count frequency of appearance of each character
	// and store it in a map
	unordered_map<char, int> freq;
	for (char ch : text) {
		freq[ch]++;
	}

	// Create a priority queue to store live nodes of
	// Huffman tree;
	priority_queue<Node*, vector<Node*>, comp> pq;

	// Create a leaf node for each character and add it
	// to the priority queue.
	for (auto pair : freq) {
		pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
	}



	// do till there is more than one node in the queue
	while (pq.size() != 1)
	{
		// Remove the two nodes of highest priority
		// (lowest frequency) from the queue
		Node *left = pq.top(); pq.pop();
		Node *right = pq.top();	pq.pop();

		// Create a new internal node with these two nodes
		// as children and with frequency equal to the sum
		// of the two nodes' frequencies. Add the new node
		// to the priority queue.
		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
	}

	// root stores pointer to root of Huffman Tree
	Node* root = pq.top();


	// traverse the Huffman Tree and store Huffman Codes
	// in a map. Also prints them
	unordered_map<char, string> huffmanCode;
	encode(root, "", huffmanCode);

	

	cout << "Huffman Codes are :\n" << '\n';
	for (auto pair : huffmanCode) {
		cout << pair.first << " " << pair.second << '\n';
		
	}

	return huffmanCode;
}

string encode(unordered_map<char, string> huffmanCode,string text) {

	// print encoded string
	string str = "";
	for (char ch : text) {
		str += huffmanCode[ch];
	}
	return str;
}

int main()
{
	string text = "";
	ifstream file("example.txt");
	char c;
	while (file.get(c))
		text += c;
	file.close();

	unordered_map<char, string> huffmanCode = buildHuffmanTree(text);
	string bits = encode(huffmanCode, text);
	//cout << "\nbits1 = " << bits <<" "<<bits.length()<< endl;
	string  encode_str = "";
	int mod = bits.length() % 8;
	int sum = 0;

	if (mod != 0) 
		for (int i = 0; i <8- mod; i++) 
			bits += "0";
	cout << "\nmod = " <<  mod << endl;
	cout << "\nbits2 = " << bits<<"   "<< bits.length()%8<<"    "<< bits.length() / 8 << endl;
	for (int i = bits.length()-1,power=0; i >=0; i--,power++) {
		if (bits[i] == '1') {
			sum += (int) pow(2, power);
		}
		if(i%8==0){
			char c = ((char)sum);
			encode_str = c + encode_str;
			sum = 0;
			power = -1;
		}
	}
	
	cout << encode_str<<"  "<<encode_str.length();

	ofstream comp_file("compress.txt");
	for (auto pair : huffmanCode) {
		comp_file << pair.first << " " << pair.second << '\n';
	}
	comp_file << "***\n";

	if (mod != 0)
		comp_file << mod;
	else
		comp_file << "0";

	for (int i = 0; i < encode_str.length(); i++)
		comp_file << encode_str[i];

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
