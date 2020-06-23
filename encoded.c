#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
#define MAX_INDEX 255

//자기소개 줄 수
int description_num = 0;

//유저 구조체
typedef struct {
	char id[MAX_INDEX];
	char name[MAX_INDEX];
	char sex[MAX_INDEX];
	unsigned char age;
	unsigned char hp;
	unsigned char mp;
	unsigned short coin;
}User;

int friend_num = 0;       //친구수
int item_num = 0;         //아이템 개수

//아이템 구조체
typedef struct {
	char name[30]; //아이템 이름
	unsigned char num; //아이템의 갯수
}Item;

//자기소개 구조체
typedef struct {
	char contents[1000]; //내용
	int text_len;        //줄에 해당하는 단어 개수
}Description;

//테스트 파일의 항목 제목 제거
void string_cut(char* str) {
	char* ptr = strtok(str, " "); //: 바로 다음에 공백으로 자름
	ptr = strtok(NULL, "\n"); //문자열 끝나면 자름
	strcpy(str, ptr); //:랑 \n 사이 문자열 저장
}


//파일에서 정보 저장하기
void load_file(char* argv, User* player, Item* tem, User* friends, Description* text) {

	FILE* fp;
	//실질적 데이터를 저장하기 위한 임시 변수들
	char buf[BUFSIZ];
	int count = 0;
	int i = 0;
	int j = 0;
	int x = 0;
	int text_num = 0;
	char item_list[6][30] = { "BOMB", "POTION", "CURE", "BOOK", "SHIELD", "CANNON" };
	int temp;

	//오류검출
	if ((fp = fopen(argv, "r")) == NULL) {
		fprintf(stderr, "파일을 열 수 없습니다.\n");
		exit(1);
	}

	//패킷 순서대로 저장
	while (count <= 4) {

		if (fscanf(fp, " %[^\n]", buf) == EOF) {
			break;
		}//파일의 끝이면 저장 종료


		if (buf[0] == '*') count++; //*로 시작하면 count 증가

		else {
			//1일 때 유저정보 저장
			if (count == 1) { //count가 1일 때
				string_cut(buf); //: 다음부터 한줄 끝날 때까지 잘라옴
				strcpy(player->id, buf); //id에 저장
				fscanf(fp, " %[^\n]", player->name);
				string_cut(player->name); //제목 잘라 저장

				fscanf(fp, " %[^\n]", player->sex); //위와 동일
				string_cut(player->sex);


				fscanf(fp, " %[^\n]", buf);
				string_cut(buf);
				player->age = atoi(buf); //숫자로 변환하여 저장


				fscanf(fp, " %[^\n]", buf);
				string_cut(buf);
				player->hp = atoi(buf);


				fscanf(fp, " %[^\n]", buf);
				string_cut(buf);
				player->mp = atoi(buf);


				fscanf(fp, " %[^\n]", buf);
				string_cut(buf);
				player->coin = atoi(buf);

			}
			//2일 때 아이템 저장
			else if (count == 2) {

				while (buf[0] != '*') { //다음 패킷 시작인 *이 나오면 종료

					//아이템 이름 저장
					strcpy((tem + i)->name, strtok(buf, ":"));

					for (j = 0; j < 6; j++) { //아이템 리스트에 아이템 이름 찾아서 \n로 초기화
						if (!strcmp((tem + i)->name, item_list[j])) strcpy(item_list[j], "\n");
					}
					temp = atoi(strtok(NULL, "\n")); //아이템 개수 정수형으로 저장
					(tem + i)->num = temp; //문자열로 아이템 개수 저장

					item_num++;

					fscanf(fp, " %[^\n]", buf);
					i++;

				}

				//나머지 아이템 개수 0으로 저장
				for (j = 0; j < 6; j++) {
					if (strcmp(item_list[j], "\n")) {
						strcpy((tem + i)->name, item_list[j]);
						(tem + i)->num = 0;
						i++;
					}
				}

				count++;
			}
			//친구 저장
			else if (count == 3) {


				while (buf[0] != '*') {


					//아이디
					string_cut(buf);//친구는 띄어쓰기 2번 해야 돼!
					string_cut(buf);//: 다음부터 한줄 끝날 때까지 잘라옴
					strcpy(friends[friend_num].id, buf); //id에 저장

					//이름
					fscanf(fp, " %[^\n]", friends[friend_num].name);
					string_cut(friends[friend_num].name);
					string_cut(friends[friend_num].name);
					//성별
					fscanf(fp, " %[^\n]", friends[friend_num].sex);
					string_cut(friends[friend_num].sex);
					string_cut(friends[friend_num].sex);
					//나이
					fscanf(fp, " %[^\n]", buf);
					string_cut(buf);
					string_cut(buf);
					friends[friend_num].age = atoi(buf);

					friend_num++;

					fscanf(fp, " %[^\n]", buf);

				}

				count++;

			}
			//자기소개
			else if (count == 4) {
				//줄마다 자기소개 구조체 배열에 저장~
				strcpy(text[x].contents, buf);
				x++;
				description_num++;

			}

		}

	}


	//파일 닫기
	fclose(fp);

}

//이름의 흔한 성을 축약하는 함수
void change_name(char* name) {
	//흔한 성 BEST7
	const char* l_name[7] = { "KIM", "LEE", "PARK", "HONG","CHOI", "JUNG", "KANG" };
	//각 성을 대치할  소문자 테이블
	char* i_name[7] = { "k", "l", "p", "h", "c", "j", "n" };
	int i, check = 0;
	char* buf = malloc(sizeof(char));
	char* name_buf = (char*)calloc(MAX_INDEX, sizeof(char));
	int space = 0;
	buf = strtok(name, " ");
	while (buf != NULL) {
		check = 0;
		for (i = 0; i < 7; i++) {
			if (!strcmp(buf, l_name[i])) { //buf와 l_name이 같으면
				check = 1; //check 를 1로 바꿈
				space = 0;
				break;
			}
		}
		if (check) { //check 가 1이면
			strcat(name_buf, i_name[i]); //i_name에 저장
		}
		else {  //check가 0이면
			if(space) strcat(name_buf, " ");
			strcat(name_buf, buf); //buf를 저장
			space = 1;
		}
		buf = strtok(NULL, " ");
	}
	strcpy(name, name_buf);
}

//플레이어의 셩별과 나이를 한 바이트로 저장하기 위하여 둘을 합치는 함수
void sexcontents(User* player) {

	if (player->sex[0] == 'F') {   //여자면 나이에 100
		player->age += 100;
	}
	//if (player->sex == 'M')

}

//친구의 성별과 나이도 위와 동일하게 합침
void fsexcontents(User* player, int index) {

	if (player[index].sex[0] == 'F') {   //여자면 나이에 100
		player[index].age += 100;
	}
	//if (player->sex == 'M')

}

//아이디 중복 단어 압축
void change_id(char* id) {
	int i = 0, j = 0;
	int index = 0;
	int len = strlen(id);
	for (int i = 0; i < len; i++)
		if (id[i] == 'K' && id[i + 1] == 'M' && id[i + 2] == 'U') {
			if (id[i + 3] == '_' && id[i + 4] == 'C' && id[i + 5] == 'S') {
				id[i] = 's';
				index = i;
				for (j = index + 1; j < len; j++) {
					id[j] = id[j + 5];
				}
			}
			else {
				id[i] = 'u';
				index = i;
				for (j = index + 1; j < len; j++) {
					id[j] = id[j + 2];
				}
			}
		}
}


void upload_file(char *argv, User* player, char* en_tem, Item* tem, User* friends, Description* text) {

	FILE* fin; //파일 오픈
	fin = fopen(argv, "wb");

	if (fin == NULL) {
		fprintf(stderr, "업로드 파일 오프 에러\n");
		exit(1);
	}

	unsigned char* sbuf = NULL;
	unsigned char str_len = 0;

	//플레이어 아이디
	change_id(player->id);
	str_len = strlen(player->id);
	sbuf = (unsigned char*)malloc(sizeof(unsigned char*) * str_len);   // 아이>디 저장할 동적배열 선언
	strcpy(sbuf, player->id); // 아이디 문자열 버퍼에 저장

	fwrite(&str_len, sizeof(unsigned char), 1, fin); //아이디 길이 씀
	fwrite(&str_len, sizeof(unsigned char), 1, fin); //아이디 길이 씀
	fwrite(&str_len, sizeof(unsigned char), 1, fin); //아이디 길이 씀
	//fwrite(str_len, sizeof(unsigned char), 1, fin); //아이디 길이 2번 씀->나>중에 변조 확인 하려면
	fwrite(sbuf, sizeof(unsigned char), str_len, fin);
	fwrite(sbuf, sizeof(unsigned char), str_len, fin);
	fwrite(sbuf, sizeof(unsigned char), str_len, fin);

	free(sbuf);
	sbuf = (unsigned char*)malloc(sizeof(unsigned char*) * str_len);   // 이름 저장할 동적배열 선언
	change_name(player->name);
	str_len = strlen(player->name); // 이름 배열에 저장
	strcpy(sbuf, player->name); // 아이디 문자열 버퍼에 저장

	fwrite(&str_len, sizeof(unsigned char), 1, fin); //이름 길이 씀
	fwrite(&str_len, sizeof(unsigned char), 1, fin);
	fwrite(&str_len, sizeof(unsigned char), 1, fin);

	fwrite(sbuf, sizeof(unsigned char), str_len, fin);  //이름 입력
	fwrite(sbuf, sizeof(unsigned char), str_len, fin);
	fwrite(sbuf, sizeof(unsigned char), str_len, fin);
	free(sbuf);

	sexcontents(player); //나이와 성병을 합치는 함수
	fwrite(&player->age, sizeof(unsigned char), 1, fin); // 나이  + 성별 저장
	fwrite(&player->age, sizeof(unsigned char), 1, fin);
	fwrite(&player->age, sizeof(unsigned char), 1, fin);

	fwrite(&player->hp, sizeof(unsigned char), 1, fin); //hp 길이 씀
	fwrite(&player->hp, sizeof(unsigned char), 1, fin); //hp 길이 씀
	fwrite(&player->hp, sizeof(unsigned char), 1, fin); //hp 길이 씀

	fwrite(&player->mp, sizeof(unsigned char), 1, fin); //mp 길이 씀
	fwrite(&player->mp, sizeof(unsigned char), 1, fin); //mp 길이 씀
	fwrite(&player->mp, sizeof(unsigned char), 1, fin); //mp 길이 씀

	fwrite(&player->coin, sizeof(unsigned short), 1, fin); //coin 길이 씀
	fwrite(&player->coin, sizeof(unsigned short), 1, fin); //coin 길이 씀
	fwrite(&player->coin, sizeof(unsigned short), 1, fin); //coin 길이 씀

	//항목 구분 할까 말까??

	sbuf = (unsigned char*)malloc(sizeof(unsigned char*) * item_num);   //아이>템 이름 저장할 동적배열 선언
	strcpy(sbuf, en_tem); // 아이템 문자열 버퍼에 저장 abcdabcdabdc
	fwrite(&item_num, sizeof(unsigned char), 1, fin);
	fwrite(&item_num, sizeof(unsigned char), 1, fin);
	fwrite(&item_num, sizeof(unsigned char), 1, fin);

	fwrite(sbuf, sizeof(unsigned char), item_num, fin);
	fwrite(sbuf, sizeof(unsigned char), item_num, fin);
	fwrite(sbuf, sizeof(unsigned char), item_num, fin);
	free(sbuf);
	//아이템 갯수 3번 저장 ! 10 10 10 5 5 5 3 3 3 200 200 200
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < item_num; i++) {
			fwrite(&tem[i].num, sizeof(unsigned char), 1, fin);
		}
	}

	//친구 아이디
	unsigned char fr_len = friend_num; //친구 수 저장
	fwrite(&fr_len, sizeof(unsigned char), 1, fin);
	fwrite(&fr_len, sizeof(unsigned char), 1, fin);
	fwrite(&fr_len, sizeof(unsigned char), 1, fin);


	for (int i = 0; i < friend_num; i++) {
		change_id(friends[i].id);
		str_len = strlen(friends[i].id);
		sbuf = (unsigned char*)malloc(sizeof(unsigned char*) * str_len);   // 아>이디 저장할 동적배열 선언
		strcpy(sbuf, friends[i].id); // 아이디 문자열 버퍼에 저장


		fwrite(&str_len, sizeof(unsigned char), 1, fin);
		fwrite(&str_len, sizeof(unsigned char), 1, fin);
		fwrite(&str_len, sizeof(unsigned char), 1, fin); //아이디 길이 씀

		fwrite(sbuf, sizeof(unsigned char), str_len, fin);
		fwrite(sbuf, sizeof(unsigned char), str_len, fin);
		fwrite(sbuf, sizeof(unsigned char), str_len, fin); //아이디 문자열 저장
		free(sbuf);

		sbuf = (unsigned char*)malloc(sizeof(unsigned char*) * str_len);   // 이름 저장할 동적배열 선언
		change_name(friends[i].name);
		str_len = strlen(friends[i].name); // 이름 배열에 저장
		strcpy(sbuf, friends[i].name); // 아이디 문자열 버퍼에 저장

		fwrite(&str_len, sizeof(unsigned char), 1, fin); //이름 길이 씀
		fwrite(&str_len, sizeof(unsigned char), 1, fin); //이름 길이 씀
		fwrite(&str_len, sizeof(unsigned char), 1, fin); //이름 길이 씀

		fwrite(sbuf, sizeof(unsigned char), str_len, fin);
		fwrite(sbuf, sizeof(unsigned char), str_len, fin);
		fwrite(sbuf, sizeof(unsigned char), str_len, fin); // 이름 저장
		free(sbuf);

		fsexcontents(friends, i); //나이와 성병을 합치는 함수
		fwrite(&friends[i].age, sizeof(unsigned char), 1, fin); //성별+나이 저장 (F/M)
		fwrite(&friends[i].age, sizeof(unsigned char), 1, fin);
		fwrite(&friends[i].age, sizeof(unsigned char), 1, fin);

	}

	// 자기소개 그냥 친구 목록 디코드 끝나면 자기소개는 끝까지 긁어올 거임

	unsigned short text_len = text[0].text_len - 1; // 자기소개 총 길이 변조 할거면 적고 -> 자기소개 encording 에서 총 줄 수 저장하는 코드 있음
	sbuf = (unsigned char*)malloc(sizeof(unsigned char*) * text_len);   // 내용 저장할 동적배열 선언


	fwrite(&text_len, sizeof(unsigned short), 1, fin);//자소 전체 길이 쓰기
	fwrite(&text_len, sizeof(unsigned short), 1, fin);
	fwrite(&text_len, sizeof(unsigned short), 1, fin);

	strcpy(sbuf, text[0].contents);   // 첫 문장만 따로 저장해주고

	for (int i = 1; i < description_num; i++) {
		int len = strlen(text[i].contents);  // 자기소개 길이만큼
		strncat(sbuf, text[i].contents, len); // 동적 배열에 자기소개 이어서 저장
	}

	fwrite(sbuf, sizeof(unsigned char), text_len, fin);  // 마지막 엔터 공백 제거하기 위해 -2
	fwrite(sbuf, sizeof(unsigned char), text_len, fin);
	fwrite(sbuf, sizeof(unsigned char), text_len, fin);
	free(sbuf);



	fclose(fin);


}



void encoding_tem(Item* tem, char* en_tem) {

	int i;

	for (i = 0; i < item_num; i++) {

		en_tem[i] = tem[i].name[2];

	}



}

void encoding_description(Description* text) {


	int i, j, x, alpha_num, text_string;
	char* fs = NULL;
	char* ps = NULL;

	for (i = 1; i < description_num; i++) {    // 줄끼리 비교

		for (j = 0; j < i; j++) {

			if (strcmp(text[i].contents, text[j].contents) == 0) {
				sprintf(text[i].contents, "{%d", j);   //줄끼리 같으면 그 줄 내용에 >똑같은 문자열있는 줄 번호수 저장 -> 엔터로 구분하니까 특수문자 뒤에는 줄번호>를 가리키는 걸로 인식
				break;
			}
		}
	}


	text[0].text_len = 0; // 자기소개 모든 글자 수 저장할 변수 선언

	for (i = 0; i < description_num; i++) {
		int y;

		for (int j = 0; j < strlen(text[i].contents); j++) {  //AABBBAADDEEGAGG -> a 줄임


			if (text[i].contents[j] == 'A' && text[i].contents[j + 1] == 'A' && text[i].contents[j + 2] == 'B' && text[i].contents[j + 3] == 'B' && text[i].contents[j + 4] == 'B' && text[i].contents[j + 5] == 'A' && text[i].contents[j + 6] == 'A' && text[i].contents[j + 7] == 'D' && text[i].contents[j + 8] == 'D' && text[i].contents[j + 9] == 'E' && text[i].contents[j + 10] == 'E' && text[i].contents[j + 11] == 'G' && text[i].contents[j + 12] == 'A' && text[i].contents[j + 13] == 'G' && text[i].contents[j + 14] == 'G') {
				text[i].contents[j] = 'a';
				y = j;

				for (y = y + 1; y < strlen(text[i].contents); y++) {
					text[i].contents[y] = text[i].contents[y + 14];
				}

			}

		}


	}

	for (i = 0; i < description_num; i++) {        // 한 줄 안에서 똑같은 알파>벳 연속적으로 나오면 특수기호 붙이면서 저장


		char copy_contents[1000] = { 0, };   // 구조체 배열안에 넣기 위해 임시로 저장해 놓은 곳
		text_string = strlen(text[i].contents);   // 한 줄의 길이 저장
		// text[0].text_len += text_string; // 한 줄 길이 누적해서 저장

		alpha_num = 1;  //알파벳이 몇개인지 알기위한 변수 선언
		x = 0;



		for (j = 0; j < text_string; j++) {    // 한줄 안 연속된 알파벳 있는지 비교
			if (text[i].contents[j] == text[i].contents[j + 1])  //연속된 알파벳 있으면 변수 증가
				alpha_num++;

			else if (text[i].contents[j] != text[i].contents[j + 1]) {  // 이웃의 >알파벳이 다를 때

				if (alpha_num == 1) {  // 알파벳이 한개일 때

					copy_contents[x] = text[i].contents[j];   //임시저장할 곳에 저장
					x++;
					alpha_num = 1;  //다음 알파벳 위해 초기화

				}

				else if (alpha_num == 2) {  // 알파벳이 두개 연속으로 나올때 어차피 >용량은 같으니까 특수배열로 안바꿈

					copy_contents[x] = text[i].contents[j];   // 알파벳 두개 임시배열에 저장
					x++;
					copy_contents[x] = text[i].contents[j];
					x++;
					alpha_num = 1;
				}

				else if (alpha_num > 2 && alpha_num <=9) {  // 연속된 알파벳이 3 이상일 때

					switch (alpha_num) {

						case 3:  //연속된 알파벳이 3개일 때
							copy_contents[x] = '#';  //연속된 알파벳이 3개일 때 특수문자 # 저장
							alpha_num = 1;
							break;
						case 4:
							copy_contents[x] = '$';  //연속된 알파벳이 4개일 때 특수문자 $ 저장
							alpha_num = 1;
							break;
						case 5:
							copy_contents[x] = '%';//연속된 알파벳이 5개일 때 특수문자 % 저장
							alpha_num = 1;
							break;
						case 6:
							copy_contents[x] = '^';//연속된 알파벳이 6개일 때 특수문자 ^ 저장
							alpha_num = 1;
							break;
						case 7:
							copy_contents[x] = '&';//연속된 알파벳이 7개일 때 특수문자 & 저장
							alpha_num = 1;
							break;
						case 8:
							copy_contents[x] = '*';//연속된 알파벳이 8개일 때 특수문자 * 장
							alpha_num = 1;
							break;
						case 9:
							copy_contents[x] = '(';//연속된 알파벳이 9개일 때 특수문자 ( 저장
							alpha_num = 1;
							break;

						default:	
							printf("스위치 오류\n");
					}

					x++;
					copy_contents[x] = text[i].contents[j];
					x++;

				}
				else if(alpha_num>=10){
					for(int k = 0 ; k < alpha_num; k++){
					copy_contents[x++]=text[i].contents[j];
					}

					alpha_num = 1;	
				}
				}
		}
		copy_contents[x++] = '\n';
		strcpy(text[i].contents, copy_contents);  // 다 저장된 임시배열 원래 구조체 배열에 저장
	}

	for (i = 0; i < description_num; i++) {
		int y;

		for (int j = 0; j < strlen(text[i].contents); j++) {  //BBC -> b로 줄임

			if (text[i].contents[j] == 'B' && text[i].contents[j + 1] == 'B' && text[i].contents[j + 2] == 'C') {
				text[i].contents[j] = 'b';
				y = j;

				for (y = y + 1; y < strlen(text[i].contents); y++) {
					text[i].contents[y] = text[i].contents[y + 2];
				}

			}

		}



		for (int j = 0; j < strlen(text[i].contents); j++) {  //HELLO -> o로 줄임

			if (text[i].contents[j] == 'H' && text[i].contents[j + 1] == 'E' && text[i].contents[j + 2] == 'L' && text[i].contents[j + 3] == 'L' && text[i].contents[j + 4] == 'O') {
				text[i].contents[j] = 'o';
				y = j;

				for (y = y + 1; y < strlen(text[i].contents); y++) {
					text[i].contents[y] = text[i].contents[y + 4];
				}

			}

		}
		for (int j = 0; j < strlen(text[i].contents); j++) {  //ZZKKZZKKKKKK -> z로 줄임

			if (text[i].contents[j] == 'Z' && text[i].contents[j + 1] == 'Z' && text[i].contents[j + 2] == 'K' && text[i].contents[j + 3] == 'K' && text[i].contents[j + 4] == 'Z' && text[i].contents[j + 5] == 'Z' && text[i].contents[j + 6] == '^' && text[i].contents[j + 7] == 'K') {
				text[i].contents[j] = 'z';
				y = j;

				for (y = y + 1; y < strlen(text[i].contents); y++) {
					text[i].contents[y] = text[i].contents[y + 7];
				}

			}

		}
	}

	for (int i = 0; i < description_num; i++)
		text[0].text_len += strlen(text[i].contents);


}

int main(int argc, char *argv[]) {
	if(argc != 3){
		printf("usage) ./encoder.out sample.txt encoded_data\n");
		return 0;
	}

	User player;//플레이어 한 명
	Item tem[6]; //템
	User friends[100];//친구
	Description text[50]; //자기소개
	char en_tem[12] = "";


	load_file(argv[1], &player, tem, friends, text);//텍스트 파일 열기
	encoding_tem(tem, en_tem);

	encoding_description(text);
	upload_file(argv[2], &player, en_tem, tem, friends, text);

	return 0;
}
