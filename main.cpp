#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
using namespace std;

string getfolder() {
    filesystem::path Ordnerpath = filesystem::current_path() / "Ordner";
    string Ordnerstring = Ordnerpath.string();
    return Ordnerstring;
}

int main() {




}