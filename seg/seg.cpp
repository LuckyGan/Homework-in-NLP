/*
 * 作者：MF1733012 甘让兴
 * 描绘：自然语言处理课程Project2（汉语自动分词实现）
 * 时间：2017-11-04
 */
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
using std::ifstream;
using std::multiset;
using std::string;
using std::vector;
using std::reverse;
using std::cout;
using std::endl;
using std::cin;

using compareType = bool(*)(const string&, const string&);
/*
 * multiset排序函数,按词语长度逆向排序
 */
bool compareWordSize(const string &lhs, const string &rhs) {
	return lhs.size() > rhs.size();
}

/*
 * 初始化词语multiset,根据词语长度逆向排序
 */
void initWordSet(multiset<string, compareType> &wordSet) {
	ifstream ifs("ce（ms-word）.txt");
	if (!ifs) {
		cout << "open file error!" << endl;
		exit(1);
	}
	string line;
	string::size_type end;
	while (getline(ifs, line)) {
		end = line.find_first_of(",");
		wordSet.insert(line.substr(0, end));
	}
	ifs.close();
}

/*
 * 从左至右(FMM)取最长的词
 */
void FMM(vector<string> &fmmVec, const multiset<string, compareType> &wordMultiSet, const string &sentence) {
	multiset<string, compareType>::const_iterator iter;
	string::size_type begin = 0;
	while (1) {
		iter = wordMultiSet.begin();
		bool isInsert = false;
		while (iter != wordMultiSet.end()) {
			if (begin < sentence.size() && sentence.substr(begin, iter->size()) == *iter) {
				isInsert = true;
				fmmVec.push_back(*iter);
				begin += iter->size();
				iter = wordMultiSet.begin();
				continue;
			}
			++iter;
		}
		if (!isInsert) {
			if(begin < sentence.size())
				fmmVec.push_back(sentence.substr(begin));
			break;
		}
	}
}

/*
 * 从右至左(RMM)取最长的词
 */
void RMM(vector<string> &rmmVec, const multiset<string, compareType> &wordMultiSet, const string &sentence) {
	multiset<string, compareType>::const_iterator iter;
	string::size_type begin = sentence.size();
	while (1) {
		iter = wordMultiSet.begin();
		bool isInsert = false;
		while (iter != wordMultiSet.end()) {
			if (begin - iter->size() < sentence.size() && sentence.substr(begin - iter->size(), iter->size()) == *iter) {
				isInsert = true;
				rmmVec.push_back(*iter);
				begin -= iter->size();
				iter = wordMultiSet.begin();
				continue;
			}
			++iter;
		}
		if (!isInsert) {
			if (begin > 0)
				rmmVec.push_back(sentence.substr(0, begin));
			break;
		}
	}
}

/*
 * 格式化打印Vector
 */
void displayVec(const vector<string> &svec, string segType) {
	cout << "-----------------  " << segType << "  -----------------" << endl;
	bool isFirst = true;
	for (const auto &elem : svec) {
		if (isFirst) {
			cout << elem;
			isFirst = false;
		} else {
			cout << " / " << elem;
		}
	}
	cout << endl;
	cout << "-----------------  " << segType << "  -----------------" << endl;
}

/*
 * 格式化打印句子分词结果
 */
void displayConclusion(const vector<string> &fmmVec, const vector<string> &rmmVec) {
	cout << "--------------  " << "Conclusion" << "  --------------" << endl;
	if (fmmVec == rmmVec)
		cout << "该句子分词成功！" << endl;
	else
		cout << "该句子分词失败！" << endl;
	cout << "--------------  " << "Conclusion" << "  --------------" << endl;
}

int main() {
	multiset<string, compareType> wordMultiSet(compareWordSize);
	initWordSet(wordMultiSet);
	cout << "请输入不带任何标点符号及空格的中文句子：" << endl;
	string sentence;
	vector<string> fmmVec, rmmVec;
	while (cin >> sentence) {
		fmmVec.clear();
		rmmVec.clear();
		FMM(fmmVec, wordMultiSet, sentence);
		displayVec(fmmVec, "FMM");
		RMM(rmmVec, wordMultiSet, sentence);
		reverse(rmmVec.begin(), rmmVec.end());
		displayVec(rmmVec, "RMM");
		displayConclusion(fmmVec, rmmVec);
	}
	return 0;
}
