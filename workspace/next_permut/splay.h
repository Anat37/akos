/*
 * splay.h
 *
 *  Created on: 22.02.2016
 *      Author: anatoly
 */

#pragma once


class node{
public:
	bool rev;
	int key;
	int leftlen;
	int rightlen;
	node* left;
	node* right;
	node* parent;
	void update(node* v){
		if (v == nullptr) return;
		return;
	}
};
