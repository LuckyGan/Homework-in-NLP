/*
 * 作者：MF1733012 甘让兴
 * 描绘：自然语言处理课程Project1（形态还原算法实现）
 * 时间：2017-10-27
 */
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
using std::string;
using std::ifstream;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::map;
using std::istringstream;

map<string, string> regularVerbsMap = {//规则动词还原Map
	{"s", ""},                                    
	{"es", ""},                                
	{"ies", "y"},						           
	{"ing", ""},					           
	{"ing", "e"}, 					           
	{"ying", "ie"}, 
	{"??ing", "?"},
	{"ed", ""},						           
	{"ed", "e"}, 						           
	{"ied", "y"},
	{"??ed", "?"}
};
map<string, string> irregularVerbsMap = {//不规则动词还原Map
	{"went", "go"}, 
	{"gone", "go"},
	{"sat", "sit"}
};

/*
 * 初始化单词字典Map
 */
void initWordMap(map<string, map<string, string>> &wordMap) {
	ifstream ifs("dic_ec.txt");
	string line, word, partOfSpeech, mean;
	string::size_type begin, end;
	map<string, string> attribute;
	while (getline(ifs, line)) {
		line = line.substr(0, line.find_last_not_of(0xff) + 1);
		begin = line.find_first_of(0xff);
		word = line.substr(0, begin);
		attribute.clear();
		while (begin != string::npos) {
			end = line.find_first_of( 0xff, begin + 1);
			if (end != string::npos)
				partOfSpeech = line.substr(begin + 1, end - begin - 1);
			else {//针对原始字典中，有些只有词性，却没中文释义的情况
				attribute.insert({line.substr(begin + 1), "" });
				break;
			}
			begin = end;
			end = line.find_first_of( 0xff, begin + 1);
			if (end != string::npos)
				mean = line.substr(begin + 1, end - begin - 1);
			else
				mean = line.substr(begin + 1);
			attribute.insert({partOfSpeech, mean});
			begin = end;
		}
		wordMap.insert({word, attribute});
	}
	ifs.close();
}

/*
 * 单词后缀匹配
 */
inline bool commonSuffixMatch(const string &word, const string &suffix) {
	if (word.size() >= suffix.size()) {
		if ('?' == suffix[0]) {
			if (word.substr(word.size() - suffix.size(), 1) == word.substr(word.size() - suffix.size() + 1, 1))
				return true;
			else
				return false;
		} else {
			if (word.substr(word.size() - suffix.size()) == suffix)
				return true;
			else
				return false;
		}
	} else
		return false;
}

/*
 * 打印还原单词及其属性
 */
inline void showWordInfo(map<string, map<string, string>>::const_iterator &iter) {
	cout << "---------------" << endl;
	cout << iter->first << endl;
	cout << "---------------" << endl;
	for (const auto &attribute : iter->second) {
		cout << attribute.first << " " << attribute.second << endl;
		cout << "---------------" << endl;
	}
}

/*
 * 规则动词还原
 */
void resularVerbsReduction(const string &word, const map<string, map<string, string>> &wordMap, bool &isReduction) {
	string reducitonWord;//还原后的单词
	for (auto iter = regularVerbsMap.cbegin(); iter != regularVerbsMap.cend(); ++iter) {
		if (commonSuffixMatch(word, iter->first)) {
			if ('?' == iter->first[0])
				reducitonWord = word.substr(0, word.size() - iter->first.size()).append(word.substr(word.size() - iter->first.size(), 1));
			else
				reducitonWord = word.substr(0, word.size() - iter->first.size()).append(iter->second);
			auto result = wordMap.find(reducitonWord);
			if (result != wordMap.end()) {//多项查找，不返回
				showWordInfo(result);
				isReduction = true;
			}
		}
	}
}

/*
 * 不规则动词还原
 */
void irresularVerbsReduction(string &word, const map<string, map<string, string>> &wordMap, bool &isReduction) {
	for (auto iter = irregularVerbsMap.cbegin(); iter != irregularVerbsMap.cend(); ++iter) {
		if (word == iter->first) {
			auto result = wordMap.find(iter->second);
			if (result != wordMap.end()) {//多项查找，不返回
				showWordInfo(result);
				isReduction = true;
			}
		}
	}
}

/*
 * 单词形态还原
 */
void wordReduction(string &word, const map<string, map<string, string>> &wordMap) {
	auto result = wordMap.find(word);
	if (result != wordMap.end()) {
		showWordInfo(result);
	} else {
		bool isReduction = false;
		resularVerbsReduction(word, wordMap, isReduction);//
		irresularVerbsReduction(word, wordMap, isReduction);
		if (!isReduction) {
			cout << "---------------" << endl;
			cout << "In this dictionary, we can't find word '" << word << "' or it's reduction word" << endl;
			cout << "---------------" << endl;
		}
	}
}

int main() {
	map<string, map<string, string>> wordMap;
	initWordMap(wordMap);
	cout << "you can begin input: " << endl;
	string line, word;
	istringstream iss;
	while (getline(cin, line)) {
		iss.clear();
		iss.str(line);
		while (iss >> word) {
			wordReduction(word, wordMap);
		}
	}
	return 0;
}