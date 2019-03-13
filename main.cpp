#include <bits/stdc++.h>
using namespace std;
#define INF 99999999
char TEST='A' ;
int bestMaxTest=1000;
// list photo format du file
vector<pair<char,vector<string>>> listPhoto;
void readFile(const char namefile[]);
void writeAlbum(const char namefile[]);
// hacher ensemble tags 
map<string,int> ensembleTags;
void initHache();
int hacher(string s);
vector<int> unionVector(vector<int> &V1,vector<int> &V2);
int lenghtIntersection(vector<int> &V1,vector<int> &V2);
int interest(vector<int> &s1,vector<int> &s2);
// struct photo
struct photo{
    // list des tags
    vector<int> tags;
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
// struct slideshow
struct slideshow{
    // 2 V or H
    vector<int> tags;
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
void solve();
void solve1();
int penality(slideshow &s1,slideshow &s2);
int penalityB(slideshow &s1,slideshow &s2);
int penalityC(slideshow &s1,slideshow &s2);
int penalityD(slideshow &s1,slideshow &s2);
int penalityE(slideshow &s1,slideshow &s2);
int main(){
/*
    // test A
    TEST='A'; 
    bestMaxTest = 2; 
    cerr<<"****** test A *******"<<endl;
    cerr<<"Read from file ..."<<endl;
    readFile("inputs/a_example.txt");
    solve();
    cerr<<"writing sur file outputs/A ..."<<endl;
    writeAlbum("outputs/A");
    cerr<<"end trait test A"<<endl;
    cerr<<"********** score is "<<alb.score<<" ************"<<endl;
    cerr<<endl;
*/
/*
    // test C
    TEST='C'; 
    bestMaxTest = 1000; 
    cerr<<"****** test C *******"<<endl;
    cerr<<"Read from file ..."<<endl;
    readFile("inputs/c_memorable_moments.txt");
    solve();
    cerr<<"writing sur file outputs/C ..."<<endl;
    writeAlbum("outputs/C");
    cerr<<"end trait test C"<<endl;
    cerr<<"********** score is "<<alb.score<<" ************"<<endl;
    cerr<<endl;
*/
/*
    // test D
    TEST='D';
    bestMaxTest = 1; 
    cerr<<"****** test D *******"<<endl;
    cerr<<"Read from file ..."<<endl;
    readFile("inputs/d_pet_pictures.txt");
    solve();
    cerr<<"writing sur file outputs/D ..."<<endl;
    writeAlbum("outputs/D");
    cerr<<"end trait test D"<<endl;
    cerr<<"********** score is "<<alb.score<<" ************"<<endl;
    cerr<<endl;
*/
    // test E
    TEST='E';
    bestMaxTest = 1; 
    cerr<<"****** test E *******"<<endl;
    cerr<<"Read from file ..."<<endl;
    readFile("inputs/e_shiny_selfies.txt");
    solve1();
    cerr<<"writing sur file outputs/E ..."<<endl;
    writeAlbum("outputs/E");
    cerr<<"end trait test E"<<endl;
    cerr<<"********** score is "<<alb.score<<" ************"<<endl;
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
    return (photos[i].tags).size()<(photos[j].tags).size();
}
void solve(){
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
    int numberSlides=indexPhotosH.size()+indexPhotosV.size()/2;
    // vector for savoir si on a utiliser un photos 
    vector<bool> use(photos.size(),false);
    // vector for les slides , 2*numberSlides pour que on peut ajout left et a right
    vector<slideshow> slides(2*numberSlides);
    int left=numberSlides,right = numberSlides;
    if(indexPhotosH.size()>0){
        // add first photos H
        slides[left] = slideshow(photos[indexPhotosH[0]]);
        // on a utiliser photo H
        use[indexPhotosH[0]] = true;
    }else{
        int minPenality = INF;
        int indexPhoto = -1;
        // add first photo V
        for(int j:indexPhotosV){
            if(j==indexPhotosV[0]) continue;
            int p = lenghtIntersection(photos[indexPhotosV[0]].tags,photos[j].tags);
            if(p<minPenality){
                indexPhoto = j;
                minPenality = p;
            }
        }
        slides[left] = slideshow(photos[indexPhoto],photos[indexPhotosV[0]]);
        use[indexPhotosV[0]] = true;
        use[indexPhoto] = true;
    }    
    for(int i=1;i<numberSlides;i++){
        cerr<<i<<"/"<<numberSlides<<endl;
        // inithilser penality a infinity
        int test = 0;
        int minPenality = INF;
        int indexPhoto = -1,indexPhoto2 = -1;
        bool isleft = false;
        bool isV = false;
        // photo horizontal
        for(int j:indexPhotosH){
            if(use[j]) continue;
            slideshow newSlide(photos[j]);
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
        // photo vertical
        for(int j1:indexPhotosV){
            if(use[j1]) continue;
            for(int j2:indexPhotosV){
                if(use[j2]) continue;
                if (j1==j2) break;
                slideshow newSlide(photos[j1],photos[j2]);
                // test left 
                int p = penality(slides[left],newSlide);
                if(p<minPenality){
                    isV = true;
                    isleft = true;
                    minPenality = p;
                    indexPhoto = j1;
                    indexPhoto2 = j2;
                    slides[left-1] = newSlide;
                }
                // test right
                p = penality(slides[right],newSlide);
                if(p<minPenality){
                    isV = true;
                    isleft = false;
                    minPenality = p;
                    indexPhoto = j1;
                    indexPhoto2 = j2;
                    slides[right+1] = newSlide;
                }
                // max test avec V2
                test++;
                if(bestMaxTest==test) break;            
            }
        }
        use[indexPhoto] = true;
        if(isV) use[indexPhoto2] = true;
        if(isleft){
            // left
            left--;
        }else{
            // right
            right ++;
        }
    }
    cerr<<"create Album ..."<<endl;
    alb = album(slides,left,right);
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
    int numberSlides=indexPhotosH.size()+indexPhotosV.size()/2;
    // vector for savoir si on a utiliser un photos 
    vector<bool> use(photos.size(),false);
    // vector for les slides , 2*numberSlides pour que on peut ajout left et a right
    vector<slideshow> slides(2*numberSlides);
    int left=numberSlides,right = numberSlides;
    if(indexPhotosH.size()>0){
        // add first photos H
        slides[left] = slideshow(photos[indexPhotosH[0]]);
        // on a utiliser photo H
        use[indexPhotosH[0]] = true;
    }else{
        int minPenality = INF;
        int indexPhoto = -1;
        // add first photo V
        for(int j:indexPhotosV){
            if(j==indexPhotosV[0]) continue;
            int p = lenghtIntersection(photos[indexPhotosV[0]].tags,photos[j].tags);
            if(p<minPenality){
                indexPhoto = j;
                minPenality = p;
            }
        }
        slides[left] = slideshow(photos[indexPhoto],photos[indexPhotosV[0]]);
        use[indexPhotosV[0]] = true;
        use[indexPhoto] = true;
    }
    for(int i=1;i<numberSlides;i++){
        cerr<<i<<"/"<<numberSlides<<endl;
        // inithilser penality a infinity
        int minPenality = INF;
        int indexPhoto = -1,indexPhoto2 = -1;
        bool isleft = false;
        bool isV = false;
        // photo horizontal
        for(int j:indexPhotosH){
            if(use[j]) continue;
            slideshow newSlide(photos[j]);
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
        bool isleftV = false;
        for(int j:indexPhotosV){
            if(use[j]) continue;
            slideshow newSlide(photos[j]);
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
        for(int j:indexPhotosV){
            if(use[j]) continue;
            if(j!=indexPhotoVL){
                slideshow newSlide(photos[j],photos[indexPhotoVL]);
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
                slideshow newSlide(photos[j],photos[indexPhotoVR]);
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
        use[indexPhoto] = true;
        if(isV) use[indexPhoto2] = true;
        if(isleft){
            // left
            left--;
        }else{
            // right
            right ++;
        }
    }
    cerr<<"create Album ..."<<endl;
    alb = album(slides,left,right);
}
vector<int> unionVector(vector<int> &V1,vector<int> &V2){
    /**
     * V1 and V2 doit etre trie
     * V1 and V2 est set des element sans doublan
     * return union entre V1 and V2
     **/
    vector<int> res;
    // number of element V1
    int numberV1=V1.size();
    // number of element V2
    int numberV2=V2.size();
    int indexV1=0,indexV2=0;
    while(indexV1<numberV1 && indexV2<numberV2){
        if(V1[indexV1]==V2[indexV2]){
            res.push_back(V1[indexV1]);
            indexV2++;
            indexV1++;
        }else if(V1[indexV1]<V2[indexV2]){
            res.push_back(V1[indexV1]);
            indexV1++;
        }else{
            res.push_back(V2[indexV2]);
            indexV2++;            
        }
    }
    while(indexV1<numberV1){
        res.push_back(V1[indexV1]);
        indexV1++;        
    }
    while(indexV2<numberV2){
        res.push_back(V2[indexV2]);
        indexV2++;        
    }
    return res;
}
int lenghtIntersection(vector<int> &V1,vector<int> &V2){
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
int interest(vector<int> &s1,vector<int> &s2){
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
int penalityB(slideshow &s1,slideshow &s2){
    return -1*interest(s1.tags,s2.tags);
}
int penalityC(slideshow &s1,slideshow &s2){
    return -1*interest(s1.tags,s2.tags);
}
int penalityD(slideshow &s1,slideshow &s2){
    return -1*interest(s1.tags,s2.tags);
}
int penalityE(slideshow &s1,slideshow &s2){
    return -1*interest(s1.tags,s2.tags);
}