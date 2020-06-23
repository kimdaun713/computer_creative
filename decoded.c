#define CRT_SECURE_NO_WARNINGS
#include <stdio.h>       // gets_s, printf 함수를 사용하기 위해!
#include <stdlib.h>
#include <string.h>


#define MAX_INDEX 255

int item_num = 0;
int friend_num = 0;
int wrong_description = 0; //자기소개 변조 개수 세는 변수

//자기소개 구조체
typedef struct {

	char contents[1000];
	int text_len;

}Description;


typedef struct {
	char id[MAX_INDEX];
	char name[MAX_INDEX];
	char sex[MAX_INDEX];
	int age;
	int hp, mp, coin;
}User;


//디코더 아이템 구조체
typedef struct {
	char name[30];
	int num;
}Item;

char compare_text(char a, char b, char c) {      //문자 한개 비교함수

	if (a == b) {
		if (b == c)
			return a;  //a = b = c인경우

		else {
			wrong_description++;
			return b;
		}

		// a = b 이고 a,b != c인 경우
	}

	else {
		if (b == c) {
			wrong_description++;
			return b;
		}
		// b = c이고, b,c != a인 경우

		else if (a == c) {
			wrong_description++;
			return c;
		}
		// a = c이고, a,c != b인경우
		else {
			wrong_description++;
			return b;
		}
		// 다 틀린경우
	}

}

void decoded_name(User* text, User* text_real, int name_num) { //이름 압축해제

	int i = 0, j;
	int check = 0;
	int index = 0;
	int len = strlen(text->name);
	char c[2] = {0, };


	for (j = 0; j < len; j++) {

		switch (text->name[j]) {
			case 'k':
				if (i != 0) strcat(text_real[name_num].name, " ");//i가 0이 아닐 때 띄워쓰기 넣기
				strcat(text_real[name_num].name, "KIM");
				i++;
				check = 1;
				break;
			case 'l':
				if (i != 0) strcat(text_real[name_num].name, " ");
				strcat(text_real[name_num].name, "LEE");
				i++;
				check = 1;

				i++;
				check = 1;
				break;
			case 'p':
				if (i != 0) strcat(text_real[name_num].name, " ");
				strcat(text_real[name_num].name, "PARK");
				i++;
				check = 1;
				break;
			case 'h':
				if (i != 0) strcat(text_real[name_num].name, " ");
				strcat(text_real[name_num].name, "HONG");
				i++;
				check = 1;
				break;
			case 'c':
				if (i != 0) strcat(text_real[name_num].name, " ");
				strcat(text_real[name_num].name, "CHOI");
				i++;
				check = 1;
				break;
			case 'j':
				if (i != 0) strcat(text_real[name_num].name, " ");
				strcat(text_real[name_num].name, "JUNG");
				i++;
				check = 1;
				break;
			case 'n':
				if (i != 0) strcat(text_real[name_num].name, " ");
				strcat(text_real[name_num].name, "KANG");
				i++;
				check = 1;
				break;

			default:
				if (check) strcat(text_real[name_num].name, " "); //check가 1 일때 띄워쓰기 넣기
				c[0] = text->name[j];
				strncat(text_real[name_num].name, c, 1);
				i++;
				check = 0;
				break;

		}

	}

}


void decoded_id(User* player, User* player_real, int id_num) { //id 압축해제

	int j;
	int index = 0;
	char c[2] = {0,  };
	int len = strlen(player->id);

	for (j = 0; j < len; j++) {
		switch (player->id[j]) {
			case 'u':
				strcat(player_real[id_num].id, "KMU");
				break;
			case 's':
				strcat(player_real[id_num].id, "KMU_CS");
				break;
			default:
				c[0] = player->id[j];
				strncat(player_real[id_num].id, c, 1);
				break;
		}

	}


}

int strcompare(char* name, char* str1, char* str2, char* str3) {
	int i = 0, j = 0;
	int value;
	if (!strcmp(str1, str2) && !strcmp(str1, str3) && !strcmp(str2, str3)) return 0;
	else if (!strcmp(str1, str2)) {
		printf("%s에서 변조 발생 -> 수정 완료\n", name);
		return 0;
	}
	else if (!strcmp(str1, str3)) {
		printf("%s에서 변조 발생 -> 수정 완료\n", name);
		return 0;
	}
	else if (!strcmp(str2, str3)) {
		printf("%s에서 변조 발생 -> 수정 완료\n", name);
		return 1;
	}
	else {
		return 1;
		printf("%s에서 변조 중복 발생\n", name);
	}
}



unsigned char lencompare(unsigned char buf[], char* p) {
	int count = 0;
	if (buf[0] == buf[1])
	{
		count += 1;
	}
	if (buf[0] == buf[2])
	{
		count += 2;
	}
	if (buf[1] == buf[2])
	{
		count += 3;
	}

	// 변조 여부 확인
	if (count == 6) // 세 개의 데이터가 다 같을 때
	{
		return buf[0];
	}
	else if (count == 1) // 0,1 번째만 같을 때
	{
		printf("%s에서 변조 발생 -> 수정 완료\n", p);
		return buf[0];
	}
	else if (count == 2) // 0,2 번째만 같을 때
	{
		printf("%s에서 변조 발생 -> 수정 완료\n", p);
		return buf[0];
	}
	else if (count == 3) // 1과 2번째만 같을 때
	{
		printf("%s에서 변조 발생 -> 수정 완료\n", p);
		return buf[1];
	}
	else // 다 틀릴 때
	{
		printf("%s에서 변조중복 발생\n", p);
		return buf[0];
	}

}

// 성별, 나이(user, friends), hp, mp, coin 비교 함수
unsigned short compare(unsigned short buf[], char* p) {
	int count = 0;

	if (buf[0] == buf[1])
	{
		count += 1;
	}
	if (buf[0] == buf[2])
	{
		count += 2;
	}
	if (buf[1] == buf[2])
	{
		count += 3;
	}

	// 변조 여부 확인
	if (count == 6) // 세 개의 데이터가 다 같을 때
	{
		return buf[0];
	}
	else if (count == 1) // 0,1 번째만 같을 때
	{
		printf("%s에서 변조 발생 -> 수정 완료\n", p);
		return buf[0];
	}
	else if (count == 2) // 0,2 번째만 같을 때
	{
		printf("%s에서 변조 발생 -> 수정 완료\n", p);
		return buf[0];
	}
	else if (count == 3) // 1과 2번째만 같을 때
	{
		printf("%s에서 변조 발생 -> 수정 완료\n", p);
		return buf[1];
	}
	else // 다 틀릴 때
	{
		printf("%s에서 변조 중복 발생\n", p);
		return buf[0];
	}

}
//아이템 비교해서 복구하는 함수
char* compare_item(unsigned char buf[], char* p) {
	char* D_item_temp = (char*)calloc(MAX_INDEX, sizeof(char));

	for (int i = 0; i < item_num; i++)
	{
		int count = 0;

		if (buf[i] == buf[i + item_num])
		{
			count += 1;
		}
		if (buf[i] == buf[i + item_num * 2])
		{
			count += 2;
		}
		if (buf[i + item_num] == buf[i + item_num * 2])
		{
			count += 3;
		}

		// 변조 여부 확인
		if (count == 6) // 세 개의 데이터가 다 같을 때
		{
			D_item_temp[i] = buf[i];
		}
		else if (count == 1) // 0,1 번째만 같을 때
		{
			printf("%s에서 변조 발생 -> 수정 완료\n", p);
			D_item_temp[i] = buf[i];
		}
		else if (count == 2) // 0,2 번째만 같을 때
		{
			printf("%s에서 변조 발생 -> 수정 완료\n", p);
			D_item_temp[i] = buf[i];
		}
		else if (count == 3) // 1과 2번째만 같을 때
		{
			printf("%s에서 변조 발생 -> 수정 완료\n", p);
			D_item_temp[i] = buf[i + item_num];
		}
		else // 다 틀릴 때
		{
			printf("%s에서 변조 중복 발생\n", p);
			D_item_temp[i] = buf[0];
		}
	}

	return D_item_temp;

}

//아이템 이름을 원래대로 복원하는 함수
void change_item_name(Item* item, char buf[]) {

	for (int i = 0; i < item_num; i++)
	{
		//M,T,R,O,I,N
		switch (buf[i]) {
			case 'M':
				strcpy(item[i].name, "BOMB");
				break;
			case 'T':
				strcpy(item[i].name, "POTION");
				break;
			case 'R':
				strcpy(item[i].name, "CURE");
				break;
			case 'O':
				strcpy(item[i].name, "BOOK");
				break;
			case 'I':
				strcpy(item[i].name, "SHIELD");
				break;
			case 'N':
				strcpy(item[i].name, "CANNON");
				break;
			default:
				printf("존재하지 않는 아이템입니다.\n");
		}
	}

}

// 아이템 복원 후 구조체에 저장
void save_item_num(Item *item, unsigned char* buf) {
	for (int i = 0; i < item_num; i++)
	{
		item[i].num = buf[i];
	}
	int result = 0;
	for (int i = 0; i < item_num; i++) { // 아이템 개수 다 더해서 초과했을 경우
		result += item[i].num;
	}
	if (result > 255) printf("아이템 저장 개수 초과\n");
}

int description_num = 0;

void decoded_description(Description* text, Description* myself) { //자기소개 압축해제

	int i, j, x;
	int index1 = 0;
	int index2 = 0;
	int index = 0;
	char c[2] = {0,  }, index_word;


	for (j = 0; j < text->text_len; j++) {

		switch (text->contents[j]) {

			case 'a':
				strcat(myself[description_num].contents, "AABBBAADDEEGAGG");
				break;
			case 'b':
				strcat(myself[description_num].contents, "BBC");
				break;
			case 'z':
				if (myself[description_num].contents == NULL)
					strcpy(myself[description_num].contents, "ZZKKZZKKKKKK");
				else
					strcat(myself[description_num].contents, "ZZKKZZKKKKKK");
				break;
			case 'o':
				if (myself[description_num].contents == NULL)
					strcpy(myself[description_num].contents, "HELLO");
				else
					strcat(myself[description_num].contents, "HELLO");
				break;
			case '\n':
				description_num++;
				break;
			case '#':  //연속된 알파벳이 3개일 때
				for (i = 0; i < 3; i++) {
					c[0] = text->contents[j + 1];
					strncat(myself[description_num].contents, c, 1);
				}
				j++;
				break;
			case '$':  //연속된 알파벳이 4개일 때
				for (i = 0; i < 4; i++) {
					c[0] = text->contents[j + 1];
					strncat(myself[description_num].contents, c, 1);
				}
				j++;
				break;

			case '%':  //연속된 알파벳이 5개일 때
				for (i = 0; i < 5; i++) {
					c[0] = text->contents[j + 1];
					strncat(myself[description_num].contents, c, 1);
				}
				j++;
				break;
			case '^':  //연속된 알파벳이 6개일 때
				for (i = 0; i < 6; i++) {
					c[0] = text->contents[j + 1];
					strncat(myself[description_num].contents, c, 1);
				}
				j++;
				break;
			case '&':  //연속된 알파벳이 7개일 때
				for (i = 0; i < 7; i++) {
					c[0] = text->contents[j + 1];
					strncat(myself[description_num].contents, c, 1);
				}
				j++;
				break;
			case '*':  //연속된 알파벳이 8개일 때
				for (i = 0; i < 8; i++) {
					c[0] = text->contents[j + 1];
					strncat(myself[description_num].contents, c, 1);
				}
				j++;
				break;
			case '(':  //연속된 알파벳이 9개일 때
				for (i = 0; i < 9; i++) {
					c[0] = text->contents[j + 1];
					strncat(myself[description_num].contents, c, 1);
				}
				j++;
				break;
			case'{':

				index = atoi(&text->contents[j + 1]);
				if (text->contents[j + 2] >= 48 && text->contents[j + 2] <= 57) {
					strcpy(myself[description_num].contents, myself[index].contents);
					j += 2;
				}
				else {
					strcpy(myself[description_num].contents, myself[index].contents);
					j++;
				}

				break;
			default:
				c[0] = text->contents[j];
				strncat(myself[description_num].contents, c, 1);
				break;

		}


	}

	myself[0].text_len = description_num; // 자기소개가 총 몇줄이나 있는지 저장해놓음


	if (wrong_description != 0) {
		if (wrong_description == 1)
			printf("DESCRIPTION에서 변조 발생 -> 수정 완료\n");


		else if (wrong_description >= 2)
			printf("DESCRIPTION에서 변조 중복 발생\n");
	}


}
void f_load(char* argv, User* user, Item* item, User* friends, Description* text) {

	FILE* fp = fopen(argv, "rb");
	if (fp == NULL) {
		fprintf(stderr, "FILE OPEN ERROR\n");
		exit(1);
	}

	Description text_temp[3] = { 0, };  //자기소개에 쓰임

	unsigned char ilen[3] = { 0 };
	unsigned char nlen[3] = { 0 };
	unsigned char len;
	unsigned short buff[3] = { 0, };
	unsigned char buf[MAX_INDEX];

	int index;

	//User* user = (User*)calloc(sizeof(User));
	User* user_result = (User*)calloc(1, sizeof(User));
	User* user1 = (User*)calloc(1, sizeof(User));
	User* user2 = (User*)calloc(1, sizeof(User));
	User* user3 = (User*)calloc(1, sizeof(User));


	int i = 0;
	char* p = "USER STATUS";

	//아이디 길이
	for (i = 0; i < 3; i++) {
		fread(&ilen[i], sizeof(unsigned char), 1, fp);
	}
	len = lencompare(ilen, p); //길이 비교해서 받아옴
	//유저 아이디
	fread(user1->id, sizeof(char), len, fp);
	fread(user2->id, sizeof(char), len, fp);
	fread(user3->id, sizeof(char), len, fp);
	index = strcompare(p, user1->id, user2->id, user3->id); //아이디 비교해서 받아옴
	if (index == 0) strcpy(user_result->id, user1->id);
	else if (index == 1) strcpy(user_result->id, user2->id);
	else strcpy(user_result->id, user3->id);
	decoded_id(user_result, user, 0); //아이디 디코드
	//이름 길이
	for (i = 0; i < 3; i++) {
		fread(&nlen[i], sizeof(unsigned char), 1, fp);
	}
	len = lencompare(nlen, p);
	//유저 이름
	fread(user1->name, sizeof(char), len, fp);
	fread(user2->name, sizeof(char), len, fp);
	fread(user3->name, sizeof(char), len, fp);
	index = strcompare(p, user1->name, user2->name, user3->name); //문자 비교해서 받아옴
	if (index == 0) strcpy(user_result->name, user1->name);
	else if (index == 1) strcpy(user_result->name, user2->name);
	else strcpy(user_result->name, user3->name);
	decoded_name(user_result, user, 0); //이름 디코드

	// 유저 성별 + 나이
	for (int i = 0; i < 3; i++) {
		fread(&buff[i], sizeof(unsigned char), 1, fp);
	}

	int result = compare(buff, p);
	// 여자
	if (result >= 100) {
		strcpy(user->sex, "FEMALE");
		user->age = result - 100;
		if (user->age > 99) printf("나이 초과(0~99)\n");
	}
	//남자
	else {
		strcpy(user->sex, "MALE");
		user->age = result;
	}

	// hp
	for (int i = 0; i < 3; i++) {
		fread(&buff[i], sizeof(unsigned char), 1, fp);
	}

	result = compare(buff, p);
	user->hp = result;
	// mp
	for (int i = 0; i < 3; i++) {
		fread(&buff[i], sizeof(unsigned char), 1, fp);
	}
	result = compare(buff, p);
	user->mp = result;
	// coin
	for (int i = 0; i < 3; i++) {
		fread(&buff[i], sizeof(unsigned char), 2, fp);
	}
	result = compare(buff, p);
	user->coin = result;
	for (int i = 0; i < 3; i++) {
		buff[i] = 0;
	}
	free(user_result);
	free(user1);
	free(user2);
	free(user3);

	// 아이템
	p = "ITEMS";
	for (int i = 0; i < 3; i++) { // 아이템 항목 개수
		fread(&buff[i], sizeof(unsigned char), 1, fp);
	}
	result = compare(buff, p);
	item_num = result;
	for (int i = 0; i < (3 * item_num); i++) { // 아이템 이름
		fread(&buf[i], sizeof(unsigned char), 1, fp);
	}
	char* tmp = compare_item(buf, p);
	change_item_name(item, tmp); //아이템 이름을 다시 복원
	for (int i = 0; i < MAX_INDEX; i++) {
		buf[i] = 0;
	}

	for (int i = 0; i < (3 * item_num); i++) { // 아이템 개수
		fread(&buf[i], sizeof(unsigned char), 1, fp);
	}

	char* temp = compare_item(buf, p);
	save_item_num(item, temp);    //아이템 개수를 숫자로 복원
	// 친구 목록
	p = "FRIENDS LIST";
	for (int i = 0; i < 3; i++) { // 친구 수
		fread(&buff[i], sizeof(unsigned char), 1, fp);
	}
	result = compare(buff, p);
	friend_num = result;

	for (int j = 0; j < friend_num; j++) {
		//친구 아이디
		for (i = 0; i < 3; i++) {
			fread(&ilen[i], sizeof(unsigned char), 1, fp);
		}
		len = lencompare(ilen, p);
		user_result = (User*)calloc(1, sizeof(User));
		user1 = (User*)calloc(1, sizeof(User));
		user2 = (User*)calloc(1, sizeof(User));
		user3 = (User*)calloc(1, sizeof(User));

		fread(user1->id, sizeof(char), len, fp);
		fread(user2->id, sizeof(char), len, fp);
		fread(user3->id, sizeof(char), len, fp);
		index = strcompare(p, user1->id, user2->id, user3->id);
		if (index == 0) strcpy(user_result->id, user1->id);
		else if (index == 1) strcpy(user_result->id, user2->id);
		else strcpy(user_result->id, user3->id);

		decoded_id(user_result, friends, j);

		for (i = 0; i < 3; i++) {
			fread(&nlen[i], sizeof(unsigned char), 1, fp);
		}
		len = lencompare(nlen, p);

		fread(user1->name, sizeof(char), len, fp);
		fread(user2->name, sizeof(char), len, fp);
		fread(user3->name, sizeof(char), len, fp);
		index = strcompare(p, user1->name, user2->name, user3->name);
		if (index == 0) strcpy(user_result->name, user1->name);
		else if (index == 1) strcpy(user_result->name, user2->name);
		else strcpy(user_result->name, user3->name);

		decoded_name(user_result, friends, j);

		// 친구 성별 + 나이
		for (int i = 0; i < 3; i++) {
			fread(&buff[i], sizeof(unsigned char), 1, fp);
		}

		result = compare(buff, p);
		// 친구 성별 나이 복원 및 저장
		// 여자
		if (result >= 100) {
			strcpy(friends[j].sex, "FEMALE");
			friends[j].age = result - 100;
			if (friends[j].age > 99) printf("나이 초과(0~99)\n");
		}
		//남자
		else {
			strcpy(friends[j].sex, "MALE");
			friends[j].age = result;
		}
		free(user_result);
		free(user1);
		free(user2);
		free(user3);

	}

	fread(&buff[0], sizeof(unsigned short), 1, fp);  //자기소개 총 길이 담아옴
	fread(&buff[1], sizeof(unsigned short), 1, fp);
	fread(&buff[2], sizeof(unsigned short), 1, fp);

	text->text_len = compare(buff, "DESCRIPTION");

	fread(text_temp[0].contents, sizeof(unsigned char), text->text_len, fp);
	fread(text_temp[1].contents, sizeof(unsigned char), text->text_len, fp);
	fread(text_temp[2].contents, sizeof(unsigned char), text->text_len, fp);

	//확인차



	for (int j = 0; j < text->text_len; j++) {

		text->contents[j] = compare_text(text_temp[0].contents[j], text_temp[1].contents[j], text_temp[2].contents[j]);

	}


	fclose(fp);

}

//파일에 쓰기
void f_upload(char* argv, User* user, Item* item, User friends[], Description myself[]) {
	FILE* fp = fopen(argv, "w");
	//유저 정보
	fprintf(fp, "*USER STATUS*\n");
	fprintf(fp, "ID: %s\n", user->id);
	fprintf(fp, "NAME: %s\n", user->name);
	fprintf(fp, "GENDER: %s\n", user->sex);
	fprintf(fp, "AGE: %d\n", user->age);
	fprintf(fp, "HP: %d\n", user->hp);
	fprintf(fp, "MP: %d\n", user->mp);
	fprintf(fp, "COIN: %d\n", user->coin);
	fprintf(fp, "\n");
	//아이템 정보
	fprintf(fp, "*ITEMS*\n");
	for (int i = 0; i < item_num; i++) {
		fprintf(fp, "%s: %d\n", item[i].name, item[i].num);
	}
	fprintf(fp, "\n");
	//친구 정보

	fprintf(fp, "*FRIENDS LIST*\n");
	for (int i = 0; i < friend_num; i++) {
		fprintf(fp, "FRIEND%d ID: %s\n", i + 1, friends[i].id);
		fprintf(fp, "FRIEND%d NAME: %s\n", i + 1, friends[i].name);
		fprintf(fp, "FRIEND%d GENDER: %s\n", i + 1, friends[i].sex);
		fprintf(fp, "FRIEND%d AGE: %d\n\n", i + 1, friends[i].age);
	}

	//자기 소개
	fprintf(fp, "*DESCRIPTION*\n");
	for (int i = 0; i <= description_num; i++) {
		fprintf(fp, "%s\n", myself[i].contents);
	}

}



int main(int argc, char* argv[])
{
	User* user = (User*)calloc(6, sizeof(User));
	Item item[MAX_INDEX] = { 0, };
	User friends[MAX_INDEX] = { 0, };
	Description myself[40] = { 0, };
	Description text = { 0, };



	f_load(argv[1], user, item, friends, &text);
	decoded_description(&text, myself);
	f_upload(argv[2], user, item, friends, myself);



	return 0;
}

