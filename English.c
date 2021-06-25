#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define word_number 1471

void read_dic();
char file_name[] = "dictionary.txt";
char str[word_number][128];//読み込んだ行を格納
char *word[word_number];//英単語を格納
char *mean[word_number];//英単語の意味を格納
char *wordClass[word_number];//品詞を格納
int wordClassNum[word_number];//品詞を格納する予定の場所
int matrix[16][16] = {};
int test();
int search();
int get_LCS(char *mainword,int sizemain,int numsub);
int check_range(int a,int b);
int max(int a,int b);
int sizeOfArray(char *array);
int bm_search(char *word, char *needle); // Boyer-Moore法

int main(){
  read_dic();//辞書の読み込み
  int mode;
  int repeat_flag = 0;
  while(1){
    if(repeat_flag == 0){//repeat_flagが0でない(1)の時に自動的に直前と同じモードを行うようにする
      printf("モードを選択してください : 小テストモード→ 1 英単語検索モード→ 2 アプリケーションを終わる→ 0\n");
      scanf("%d",&mode);
      while(!(mode == 1 || mode == 2 || mode == 0)){
        printf("入力エラー\n");
        scanf("%d",&mode);
      }
    }
    if(mode == 0){//アプリの終了
      break;
    }else if(mode == 1){//小テストモード
      repeat_flag = test();
    }else if(mode == 2){//英単語検索モード
      repeat_flag = search();
    }
  }
  return 0;
}

void read_dic(){//辞書の読み込み
  FILE *fp;

  fp = fopen(file_name,"r");

  if(fp == NULL){
    printf("ファイルが存在しません\n");//存在しなかったときの対処
  }
  for(int i = 0;fgets(str[i],128,fp) != NULL;i++){//1行ずつ読み込んでstrに格納
    word[i] = strtok(str[i]," ");//英単語を分割
    mean[i] = strtok(NULL," ");//意味を分割
    wordClass[i] = strtok(NULL," ");//品詞を分割
    wordClassNum[i] = atoi(wordClass[i]);//品詞をintに変換する
  }
  fclose(fp);
}

int test(){//小テストモード
  int list_wordclass[word_number];//問題と同じ品詞の単語の行番号を格納する場所
  for(int i = 0;i < word_number;i++){
    list_wordclass[i] = -1;
  }
  srand((unsigned int) time(NULL));
  int t = rand() % word_number;
  int sizemain = sizeOfArray(word[t]);
  int count = 0;

  for(int i = 0;i < word_number;i++){//同じ品詞の抽出
    if(wordClassNum[t] == wordClassNum[i] && t != i){
      list_wordclass[count] = i;
      count++;
    }
  }
  int LCS_list[4] = {-1,-1,-1,-1};
  int LCS_list_wordnum[4] = {-1,-1,-1,-1};
  for(int i = 0;i < count;i++){
    int LCSnum = get_LCS(word[t],sizemain,list_wordclass[i]);
    int check;
    int tmp;
    int tmpword;
    int tmp2;
    int tmpword2;
    for(check = 0;check < 3;check++){
      if(LCS_list[check] < LCSnum){
        break;
      }
    }
    tmp = LCS_list[check];
    tmpword = LCS_list_wordnum[check];
    LCS_list[check] = LCSnum;
    LCS_list_wordnum[check] = list_wordclass[i];
    for(int j = check + 1;j < 3;j++){
      tmp2 = LCS_list[j];
      tmpword2 = LCS_list_wordnum[j];
      LCS_list[j] = tmp;
      LCS_list_wordnum[j] = tmpword;
      tmp = tmp2;
      tmpword = tmpword2;
    }
  }
  int option[4];
  for(int i = 0;i < 3;i++){
    option[i] = LCS_list_wordnum[i];
  }
  option[3] = t;

  for(int i = 0;i < 4;i++){
    int r = rand() % 4;
    int tmpnum = option[i];
    option[i] = option[r];
    option[r] = tmpnum;
  }
  int ansnum;
  for(int i = 0;i < 4;i++){
    if(option[i] == t){
      ansnum = i;
      break;
    }
  }
  printf("以下問題\n");
  printf("%s\n",mean[t]);
  char option_char = 'a';
  for(int i = 0;i < 4;i++){
    printf("%c : %s\n",option_char,word[option[i]]);
    option_char++;
  }
  char choice;
  scanf(" %c",&choice);
  if('a' + ansnum == choice){
    printf("正解！\n");
  }else{
    printf("不正解！\n");
  }
  printf("回答\n");
  for(int i = 0;i < 4;i++){
    printf("%-14s : %s\n",word[option[i]],mean[option[i]]);
  }

  //以下、繰り返すなら１，繰り返さないなら０を返すための作業
  printf("もう一度小テストを行いますか？ (y/n)\n");
  char flag;
  scanf(" %c",&flag);
  while(!(flag == 'y' || flag == 'n')){
    printf("入力エラー\n");
    scanf(" %c",&flag);
  }
  if(flag == 'y'){
    printf("繰り返します\n");
    return 1;
  }
  return 0;
}

int search(){//英単語検索モード
  char needle[16];//検索する英単語を格納する場所
  printf("検索したい英単語を入力してください\n");
  scanf(" %s",needle);//検索文字列の入力

  for(int i = 0;i < word_number;i++){
    int pos;

    pos = bm_search(word[i],needle);
    if(pos != -1){
      printf("%s" ,word[i]);
      printf(":%s\n",mean[i]);
    }
  }
  //以下、繰り返すなら１，繰り返さないなら０を返すための作業
  printf("もう一度英単語検索を行いますか？ (y/n)\n");
  char flag;
  scanf(" %c",&flag);
  while(!(flag == 'y' || flag == 'n')){
    printf("入力エラー\n");
    scanf(" %c",&flag);
  }
  if(flag == 'y')return 1;
  return 0;
}

int get_LCS(char *mainword,int sizemain,int numsub){
  int sizesub = sizeOfArray(word[numsub]);
  for(int i = 0;i < sizemain;i++){
    for(int j = 0;j < sizesub;j++){
      if(mainword[i] == word[numsub][j]){
        matrix[i][j] = check_range(i- 1,j - 1) + 1;
      }else{
        matrix[i][j] = max(check_range(i - 1,j),check_range(i,j - 1));
      }
    }
  }
  return matrix[sizemain - 1][sizesub - 1];
}

int check_range(int a,int b){
  if(a >= 0 && b >= 0)return matrix[a][b];
  return 0;
}

int max(int a,int b){
  if(a > b)return a;
  return b;
}

int sizeOfArray(char *array){
  int i = 0;
  while(array[i] != '\0'){
    i++;
  }
  return i;
}

int bm_search(char *word, char *needle){
  int table[128];
  for(int i = 0; i < 128; i++){
    table[i] = sizeOfArray(needle);
  }
  for(int i = 0; i < sizeOfArray(needle)-1; i++){
    table[(int)needle[i]] = sizeOfArray(needle)-i-1;
  }

  int attention = sizeOfArray(needle)-1;//テキスト注目点の初期化
  int firstAttention = attention;//パターン注目点の初期化

  while(attention < sizeOfArray(word)){
    int j = sizeOfArray(needle)-1;
    while(word[attention] == needle[j]){
      if(j == 0){
        return attention;
      }
      attention--;
      j--;
    }
    attention += table[(int)word[attention]];

    if(attention > firstAttention){
      firstAttention = attention;
    }else{
      firstAttention += 1;
      attention = firstAttention;
    }
  }
  return -1;
}
