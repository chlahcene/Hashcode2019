#include <bits/stdc++.h>
using namespace std;
#define INF 99999999
// list photo format du file
vector<pair<char,vector<string>>> listPhoto;
void readFile(const char file[]);
int main(){
    
    return 0;
}
void readFile(const char namefile[]){
    /** readf from file and save sur @listPhot **/
  ifstream file;
  file.open (namefile);
  if (file.is_open()){
     listPhoto.clear();
      int numberPhotos,numberTags;
      // H or V
      char typePhoto;
      file >> numberPhotos;
      for(int i=0;i<numberPhotos;i++){
          file>>typePhoto>>numberTags;
          vector<string> tags(numberTags);
          for(int j=0;j<numberTags;j++){
              file>>tags[j];
          }
          listPhoto.push_back({typePhoto,tags});
      }
    file.close();
  }else {
    cerr << "Unable to open file "<<namefile<<endl;
    exit(-1);
  }
}