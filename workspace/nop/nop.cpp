//============================================================================
// Name        : nop.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

int main() {
	string s, t;
	cin >> s >> t;
	int len = t.length();
	vector<int> tind[len + 1];

	for (int i = 0; i < len; ++i)
	{
		for (int j = 0; j < s.length(); ++j)
			if (s[j] == t[i]) tind[i].push_back(j);
	}

	int ans[len + 1][len + 1];
	int d[len];
	int INF = 1000000000;

	for (int l = 0; l < len; ++l)
	{
		memset(d, INF, sizeof(int) * len );
		d[0] = -INF;
		int max = 0;

		for (int r = l; r < len; ++r){
			int ind = max;
			for (int i = tind[r].size() - 1; i >= 0; --i)
			{
				while (tind[r][i] <= d[ind] && ind >= 0) --ind;
				d[ind + 1] = tind[r][i];
				if (ind == max) ++max;
			}
			ans[l][r] = max;
			cout << max << ' ';
		}
		cout << endl;
	}

	return 0;
}
