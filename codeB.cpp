/***
 * Team codebreak
 *      Lahcene CHAKLALA
 *      Baha eddine Bouchikhi
 *      Ismail KHERBACH
 *      Youness MIMENE
 * 
 * Test B 
 *      Score 1807
 *      time 0min 11s 179 ms
 * 
 ***/
#include <bits/stdc++.h>
using namespace std;
#define INF 99999999
typedef vector<int> TypeTags;
int bestMaxTest=1000;
// vector adj for test B
vector<set<int>> adjIndex;
// list photo format du file
vector<pair<char,vector<string>>> listPhoto;
void readFile(const char namefile[]);
void writeAlbum(const char namefile[]);
// hacher ensemble tags 
map<string,int> ensembleTags;
void initHache();
int hacher(string s);
TypeTags unionVector(TypeTags &V1,TypeTags &V2);
int lenghtIntersection(TypeTags &V1,TypeTags &V2);
int interest(TypeTags &s1,TypeTags &s2);
// struct photo
struct photo{
    // list des tags
    TypeTags tags;
    int index;
    char type;
    photo(){};
    photo (int nIndex,char nType,vector<string> &nTags){
        // l'ensemble du tags doit etre inithiliser pour hacher
        index = nIndex;
        type = nType;
        for(string s:nTags){
            tags.push_back(hacher(s));
        }
        sort(tags.begin(),tags.end());
    }
};
// list des photos horizontal vertical
vector<photo> photos;
vector<int> indexPhotosH,indexPhotosV;
void createPhotos();
bool comparPhotoSize(int i,int j);
bool comparPhotoInter(int i,int j);
// struct slideshow
struct slideshow{
    // 2 V or H
    TypeTags tags;
    int index1=-1;
    int index2=-1;
    slideshow(){};
    slideshow(photo &ph){
        tags = ph.tags;
        index1 = ph.index;
    };
    slideshow(photo &ph1,photo &ph2){
        tags = unionVector(ph1.tags,ph2.tags);
        index2 = ph2.index;
        index1 = ph1.index;
    };    
    void add(photo &ph){
        tags = unionVector(tags,ph.tags);
        index2 = ph.index;
    }
};
// struct album
struct album{
    // album et son score
    vector<pair<int,int>> index;
    int score=0;
    album(){};
    album(vector<slideshow> &slides,int start,int end){
        assert(start<=end);
        for(int i=start;i<=end;i++){
            if(i!=start){
                score += interest(slides[i-1].tags,slides[i].tags);
            }
            if(slides[i].index2!=-1){
                index.push_back({slides[i].index1,slides[i].index2});
            }else{
                index.push_back({slides[i].index1,-1});
            }
        }
    }
    void addIndex(int ind){
        index.push_back({ind,-1});
        int sz=index.size();
        if(sz>1){
            score+=interest(photos[ind].tags,photos[sz-2].tags);
        }
    }
};
// album
album alb;
void solve2();
int penality(slideshow &s1,slideshow &s2);
inline unsigned long long get_time_in_ms();
int main(){
    unsigned long long current_time, difference1;
    current_time = get_time_in_ms(); 
    // test B
    cerr<<"****** test B *******"<<endl;
    cerr<<"Read from file ..."<<endl;
    readFile("inputs/b_lovely_landscapes.txt");
    solve2();
    cerr<<"writing sur file outputs/B.txt ..."<<endl;
    writeAlbum("outputs/B.txt");
    cerr<<"end trait test B"<<endl;
    cerr<<"********** score is "<<alb.score<<" ************"<<endl;
    difference1 = get_time_in_ms() - current_time;
    int minute=difference1/60000;
    int sc = (difference1%60000)/1000;
    int ms = difference1%1000;
    cerr<<"********** time is "<<minute<<"min "<<sc<<"s "<<ms<<" ms ***********"<<endl;
    cerr<<endl;
    return 0;
}
void readFile(const char namefile[]){
    /** 
     * read from file and save sur @listPhoto 
     ***/
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
void writeAlbum(const char namefile[]){
    /** 
     * write @alb sur file **/
    ofstream file;
    file.open (namefile);
    if (file.is_open()){
      file << (alb.index).size()<<endl;
      for(auto c:alb.index){
        if(c.second!=-1){
            file<<c.first<<" "<<c.second<<endl;
        }else{
            file<<c.first<<endl;
        }          
      }
    file.close();
  }else {
    cerr << "Unable to open file "<<namefile<<endl;
    exit(-1);
  }
}
void initHache(){
    /** 
     * donner a all the tags un key unique
     **/
    ensembleTags.clear();
    int key=0;
    for(auto c:listPhoto){
        for(auto cc:c.second){
            if(!ensembleTags.count(cc)){
                ensembleTags[cc]=key;
                key++;
            }
        }
    }
}
int hacher(string s){
    return ensembleTags[s];
}
bool comparPhotoSize(int i,int j){
    /**
     * comparison selon size of tags
     **/
    return (listPhoto[i].second).size()<(listPhoto[j].second).size();
}
bool comparPhotoInter(int i,int j){
    /**
     * comparison selon size of tags
     **/
    return (adjIndex[i]).size()<(adjIndex[j]).size();
}
TypeTags unionVector(TypeTags &V1,TypeTags &V2){
    /**
     * V1 and V2 doit etre trie
     * V1 and V2 est set des element sans doublan
     * return union entre V1 and V2
     **/
    // number of element V1
    int numberV1=V1.size();
    // number of element V2
    int numberV2=V2.size();
    TypeTags res(numberV1+numberV2);
    int indexV1=0,indexV2=0,numberRes=0;
    while(indexV1<numberV1 && indexV2<numberV2){
        if(V1[indexV1]==V2[indexV2]){
            res[numberRes] = V1[indexV1];
            numberRes++;
            indexV2++;
            indexV1++;
        }else if(V1[indexV1]<V2[indexV2]){
            res[numberRes] = V1[indexV1];
            numberRes++;
            indexV1++;
        }else{
            res[numberRes] = V2[indexV2];
            numberRes++;
            indexV2++;            
        }
    }
    while(indexV1<numberV1){
        res[numberRes] = V1[indexV1];
        numberRes++;
        indexV1++;        
    }
    while(indexV2<numberV2){
        res[numberRes] = V2[indexV2];
        numberRes++;
        indexV2++;        
    }
    res.resize(numberRes);
    return res;
}
int lenghtIntersection(TypeTags &V1,TypeTags &V2){
    /**
     * V1 and V2 doit etre trie
     * V1 and V2 est set des element sans doublan
     * return lenght of intersection entre V1 and V2
     **/
    // lengh of intersection
    int res=0;
    // number of element V1
    int numberV1=V1.size();
    // number of element V2
    int numberV2=V2.size();
    int indexV1=0,indexV2=0;
    while(indexV1<numberV1 && indexV2<numberV2){
        if(V1[indexV1]==V2[indexV2]){
            res++;
            indexV2++;
            indexV1++;
        }else if(V1[indexV1]<V2[indexV2]){
            indexV1++;
        }else{
            indexV2++;            
        }
    }
    return res;
}
int interest(TypeTags &s1,TypeTags &s2){
    /**
     * interest entre s1 et s2
     **/
    int inter = lenghtIntersection(s1,s2);
    int sizeS1=int(s1.size());
    int sizeS2=int(s2.size());
    return min<int>({sizeS1-inter,sizeS1-inter,inter});
}
void createPhotos(){
    /**
     * creer photo from data de file
     **/
    photos.clear();
    indexPhotosH.clear();
    indexPhotosV.clear();
    for(int i=0;i<listPhoto.size();i++){
        photos.push_back(photo(i,listPhoto[i].first,listPhoto[i].second));
        if(listPhoto[i].first=='H') indexPhotosH.push_back(i);
        else indexPhotosV.push_back(i);
    }

}
int penality(slideshow &s1,slideshow &s2){
    return -1*interest(s1.tags,s2.tags);
}
inline unsigned long long get_time_in_ms(){
    return (unsigned long long)((double(clock()) / CLOCKS_PER_SEC) * 1000);
}
void solve2(){
    // for test B
    initHache();
    // creer des photos from donnes de file
    cerr<<"Creer Photos ..."<<endl;
    createPhotos();
    // create ensemble from the tags
    int numberTags = ensembleTags.size();
    vector<vector<int>> ensemblePhotos(numberTags);
    for(int i:indexPhotosH){
        for(int j:photos[i].tags){
            ensemblePhotos[j].push_back(i);
        }
    }
    // create graph of intersection
    int numberPhotos = indexPhotosH.size();
    adjIndex.clear();
    adjIndex.assign(numberPhotos,{}); 
    for(int i=0;i<numberTags;i++){
        for(int j:ensemblePhotos[i]){
            adjIndex[j].insert(ensemblePhotos[i].begin(),ensemblePhotos[i].end());
        }
    }
    // erase i from adjIndex[i]
    for(int i=0;i<numberPhotos;i++){
        (adjIndex[i]).erase(i);
    }
    // sort list Photos selon number de tags
    cerr<<"Sort Listes Photos ..."<<endl;
    sort(indexPhotosH.begin(),indexPhotosH.end(),comparPhotoInter);
    cerr<<"Choice the Slides ..."<<endl;
    vector<int> path(numberPhotos,-1);
    cerr<<fixed<<setprecision(2);  
    for(int i=0;i<numberPhotos;i++){
        cerr<<"\r"<<(i+1)*100/double(numberPhotos)<<"%";
        int j=indexPhotosH[i];
        if(adjIndex[j].size()>0){
            vector<int> v(adjIndex[j].begin(),adjIndex[j].end());
            sort(v.begin(),v.end(),comparPhotoInter);
            int k=v[0];
            path[j] = k;
            adjIndex[k].erase(j);
            for(int ii=i+1;ii<numberPhotos;ii++){
                adjIndex[indexPhotosH[ii]].erase(k);
            }
            sort(indexPhotosH.begin()+i+1,indexPhotosH.end(),comparPhotoInter);
        }
    }
    cerr<<endl<<"create Album ..."<<endl;
    vector<int> inversePath(numberPhotos,-2),finPath;
    for(int i=0;i<numberPhotos;i++){
        if(path[i]==-1){
            finPath.push_back(i);
        }else inversePath[path[i]]=i;
    }
    // album
    alb = album();
    vector<bool> use(numberPhotos,false);
    for(int i:finPath){
        int j=i;
        while(j>=0 && !use[j]){
            alb.addIndex(j);
            use[j]=true;
            j=inversePath[j];
        }
    }
    for(int i=0;i<numberPhotos;i++){
        int j=i;
        while(j>=0 && !use[j]){
            alb.addIndex(j);
            use[j]=true;
            j=inversePath[j];
        }
    }
}