/***
 * Team codebreak
 *      Lahcene CHAKLALA
 *      Baha eddine Bouchikhi
 *      Ismail KHERBACH
 *      Youness MIMENE
 * 
 * Test E 
 *      Score 561020
 *      time 29min 11s 179 ms
 * 
 ***/
#include <bits/stdc++.h>
using namespace std;
#define INF 99999999
typedef bitset<500> TypeTags;
int bestMaxTest=1;
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
            tags[hacher(s)]=1;
        }
    }
};
// list des photos horizontal vertical
vector<photo> photos;
vector<int> indexPhotosH,indexPhotosV;
void createPhotos();
bool comparPhotoSize(int i,int j);
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
    void add(slideshow &slide){
        if(slide.index2!=-1){
            index.push_back({slide.index1,slide.index2});
        }else{
            index.push_back({slide.index1,-1});
        }
    }
};
// album
album alb;
void solve1();
int penality(slideshow &s1,slideshow &s2);
inline unsigned long long get_time_in_ms();
int main(){
    unsigned long long current_time, difference1;
    current_time = get_time_in_ms(); 
    // test E
    cerr<<"****** test E *******"<<endl;
    cerr<<"Read from file ..."<<endl;
    readFile("inputs/e_shiny_selfies.txt");
    solve1();
    cerr<<"writing sur file outputs/E.txt ..."<<endl;
    writeAlbum("outputs/E.txt");
    cerr<<"end trait test E"<<endl;
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

TypeTags unionVector(TypeTags &V1,TypeTags &V2){
    return V1 | V2;
}
int lenghtIntersection(TypeTags &V1,TypeTags &V2){
    return (V1 & V2).count();
}
int interest(TypeTags &s1,TypeTags &s2){
    /**
     * interest entre s1 et s2
     **/
    int inter = lenghtIntersection(s1,s2);
    int sizeS1=int(s1.count());
    int sizeS2=int(s2.count());
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
void solve1(){
    initHache();
    // creer des photos from donnes de file
    cerr<<"Creer Photos ..."<<endl;
    createPhotos();
    // sort list Photos selon number de tags
    cerr<<"Sort Listes Photos ..."<<endl;
    sort(indexPhotosH.begin(),indexPhotosH.end(),comparPhotoSize);
    sort(indexPhotosV.begin(),indexPhotosV.end(),comparPhotoSize);
    cerr<<"Choice the Slides ..."<<endl;
    // number of the slides
    int numberPhotoH=indexPhotosH.size(),numberPhotoV=indexPhotosV.size();
    int numberSlides=numberPhotoH+numberPhotoV/2;
    // vector for les slides , 2*numberSlides pour que on peut ajout left et a right
    vector<slideshow> slides(2*numberSlides);
    set<int> notUseIndexH,notUseIndexV;
    for(int i=0;i<numberPhotoH;i++) notUseIndexH.insert(i);
    for(int i=0;i<numberPhotoV;i++) notUseIndexV.insert(i);
    int left=numberSlides,right = numberSlides;
    if(indexPhotosH.size()>0){
        // add first photos H
        slides[left] = slideshow(photos[indexPhotosH[0]]);
        // on a utiliser photo H
        notUseIndexH.erase(0);
    }else{
        int minPenality = INF;
        int indexPhoto = -1;
        // add first photo V
        for(int j:notUseIndexV){
            if(j==0) continue;
            int p = -1*interest(photos[indexPhotosV[0]].tags,photos[indexPhotosV[j]].tags);
            if(p<minPenality){
                indexPhoto = j;
                minPenality = p;
            }
        }
        slides[left] = slideshow(photos[indexPhotosV[indexPhoto]],photos[indexPhotosV[0]]);
        notUseIndexV.erase(indexPhoto);
        notUseIndexV.erase(0);
    }
    cerr<<fixed<<setprecision(2);    
    for(int i=1;i<numberSlides;i++){
        cerr<<"\r"<<(i+1)*100/double(numberSlides)<<"%";
        // inithilser penality a infinity
        int minPenality = INF;
        int indexPhoto = -1,indexPhoto2 = -1;
        bool isleft = false;
        bool isV = false;
        // photo horizontal
        for(int j:notUseIndexH){
            slideshow newSlide(photos[indexPhotosH[j]]);
            // test left 
            int p = penality(slides[left],newSlide);
            if(p<minPenality){
                isleft = true;
                minPenality = p;
                indexPhoto = j;
                slides[left-1] = newSlide;
            }
            // test right
            p = penality(slides[right],newSlide);
            if(p<minPenality){
                isleft = false;
                minPenality = p;
                indexPhoto = j;
                slides[right+1] = newSlide;
            }            
        }
        // photos vertical 1
        int minPenalityVL = INF;
        int minPenalityVR = INF;
        int indexPhotoVL = -1,indexPhotoVR = -1;
        for(int j:notUseIndexV){
            slideshow newSlide(photos[indexPhotosV[j]]);
            // test left 
            int p = penality(slides[left],newSlide);
            if(p<minPenalityVL){
                minPenalityVL = p;
                indexPhotoVL = j;
            }
            // test right
            p = penality(slides[right],newSlide);
            if(p<minPenalityVR){
                minPenalityVR = p;
                indexPhotoVR = j;
            }            
        }
        // photos vertical 2
        for(int j:notUseIndexV){
            if(j!=indexPhotoVL){
                slideshow newSlide(photos[indexPhotosV[j]],photos[indexPhotosV[indexPhotoVL]]);
                // test left 
                int p = penality(slides[left],newSlide);
                if(p<minPenality){
                    isV = true;
                    isleft = true;
                    minPenality = p;
                    indexPhoto = j;
                    indexPhoto2 = indexPhotoVL;
                    slides[left-1] = newSlide;
                }
            }
            if(j!=indexPhotoVR){
                slideshow newSlide(photos[indexPhotosV[j]],photos[indexPhotosV[indexPhotoVR]]);
                // test right 
                int p = penality(slides[right],newSlide);
                if(p<minPenality){
                    isV = true;
                    isleft = false;
                    minPenality = p;
                    indexPhoto = j;
                    indexPhoto2 = indexPhotoVR;
                    slides[right+1] = newSlide;
                }
            }            
        } 
        if(isV){
            notUseIndexV.erase(indexPhoto);
            notUseIndexV.erase(indexPhoto2);
        }else{
            notUseIndexH.erase(indexPhoto);
        }
        if(isleft){
            // left
            left--;
        }else{
            // right
            right ++;
        }
    }
    cerr<<endl<<"create Album ..."<<endl;
    alb = album(slides,left,right);
}