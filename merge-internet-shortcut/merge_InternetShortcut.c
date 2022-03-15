#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void fileMerge(FILE*, FILE*, FILE*);
int addLink(FILE*, FILE*, FILE*, const int);
char* getTime();	//文字列

void main(){
	FILE *fp_html, *fp_url, *fp_href;
	char *filename_html = strcat(getTime(),".html");	//getTime()の返り値は文字列
	char *filename_url = "../url_all.txt", *filename_href = "../filename_all.txt";
	system("makeTmpFile.bat");	//全てのShortcutファイルを2つのtxtファイルにまとめる
	fp_html = fopen(filename_html,"w");
	fp_url = fopen(filename_url,"r");
	fp_href = fopen(filename_href,"r");

	if(fp_html!=NULL && fp_url!=NULL && fp_href!=NULL){
		fileMerge(fp_html,fp_url,fp_href);
		printf("全てのインターネットショートカットが、1つの.htmlファイルに統合されました。\n");
	}else{
		printf("ファイルがありません。\n");
	}
}

void fileMerge(FILE* fp_html, FILE* fp_url, FILE* fp_href){
	const int N = 200;

	fputs("<!DOCTYPE html><html lang=\"ja\"><head><title>",fp_html);
	fputs("ねこ",fp_html);	//htmlファイルをブラウザで開いた時のタイトル
	fputs("</title></head><body>",fp_html);
	while(addLink(fp_html, fp_url,fp_href,N) != 0);
	fputs("</body>",fp_html);

	fclose(fp_html);
	fclose(fp_url);
	fclose(fp_href);
	system("deleteTmpFile.bat");
}

int addLink(FILE* fp_html, FILE* fp_url, FILE* fp_href, const int N){
	char *tmp;
	char url[N], href[N];
	if(fgets(href, N, fp_href) != NULL){
		fgets(url, N, fp_url);	//"[InternetShortcut]"を無視するために2回やる
		fgets(url, N, fp_url);
		tmp = &url[4];	//"URL="を取り除く。4文字。
		href[strlen(href)-5] = '\0';	//末尾の".url "を取り除く。
		//printf("%s%s", href,tmp);
		fprintf(fp_html, "<a href=%s>%s</a><br>\n", tmp,href);
		return 1;
	}else{
		return 0;
	}
}

char* getTime(){
	int time_int = 0;
	char *time_str = (char*)malloc(sizeof(char)*20);
	time_t timer = time(NULL);
	struct tm *a = localtime(&timer);	//aは地方時(この国における時刻)

	sprintf(time_str,"%04d%02d%02d_%02d%02d%02d", a->tm_year+1900,a->tm_mon+1,a->tm_mday,
		a->tm_hour,a->tm_min,a->tm_sec);
	return time_str;
}