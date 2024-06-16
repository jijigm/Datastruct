// "main.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 등급
#define FAMILY 1
#define BEST 2
#define GRAND 3
#define VIP 4
#define VVIP 5

// 고객 구조체
typedef struct Customer {
    char name[80];  //이름
    int id;         //아이디
    int grade;      //등급
    unsigned int reward_points;     //적립 포인트
    unsigned int last_month_total;  //전월 실적
}Customer;

// element 타입 설정
#define ELEMENT_TYPE
typedef struct Customer element;
#include "LinkedList.h" // 이중연결리스트 코드

// 고객 정보 초기화 함수
void initializeCustomer(Customer* customer, char* name, int id) {
    strcpy(customer->name, name);
    customer->id = id;
    customer->grade = FAMILY;
    customer->reward_points = 0;
    customer->last_month_total = 0;
}
// 고객 정보 설정 함수
void setCustomer(Customer* customer, char* name, int id, int grade, 
                        unsigned int reward_points, unsigned int last_month_total) {
    strcpy(customer->name, name);
    customer->id = id;
    customer->grade = grade;
    customer->reward_points = reward_points;
    customer->last_month_total = last_month_total;
}

// 포인트 적립 함수
void getRewardPoints(Customer* customer, int payment_amount) {
    int grade = customer->grade;
    switch (grade) {
    case FAMILY:
        customer->reward_points += (int)(payment_amount * 0.01);    //unsigned int형에 daouble형 삽입 위한 (int)캐스팅
        break;
    case BEST:
        customer->reward_points += (int)(payment_amount * 0.03);
        break;
    case GRAND:
        customer->reward_points += (int)(payment_amount * 0.05);
        break;
    case VIP:
        customer->reward_points += (int)(payment_amount * 0.08);
        break;
    case VVIP:
        customer->reward_points += (int)(payment_amount * 0.10);
        break;
    }
}

// 문자열 등급 리턴 함수
const char* getGrade(int grade) {
    switch (grade) {
    case FAMILY: return "FAMILY";
    case BEST: return "BEST";
    case GRAND: return "GRAND";
    case VIP: return "VIP";
    case VVIP: return "VVIP";
    }
}
// 회원 정보 출력 함수
void printUser(DListNode* current) {
    Customer* user = &(current->data);
    printf("회원 아이디 : %d\n", user->id);
    printf("회원 이름 : %s\n", user->name);
    printf("회원 등급 : %s\n", getGrade(user->grade));
    printf("보유 포인트 : %d\n", user->reward_points);
}

// 모든 회원 출력
void printAllCustomers(DListNode* head) {
    DListNode* current = head->rlink;
    Customer* user = NULL;
    while (current != head) {
        user = &(current->data);
        printf("ID: %d, 이름: %s, 적립 포인트: %d, 고객등급: %s\n",
            user->id, user->name,
            user->reward_points, getGrade(user->grade));

        current = current->rlink;
    }
}

// 고객 ID로 고객 정보를 탐색하는 함수
DListNode* findCustomerID(DListNode* head, int id) {
    DListNode* current = head->rlink;
    while (current != head) {
        if (current->data.id == id) {
            return current;
        }
        current = current->rlink;
    }
    return NULL;
}


// 파일에서 고객 정보 읽어오기
void customersFromFile(DListNode* head, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("파일 열기 실패");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char name[80];
        int id, grade;
        unsigned int reward_points, last_month_total;

        // ','을 기준으로 구분
        char* token = strtok(line, ",");
        if (token != NULL) id = atoi(token);

        token = strtok(NULL, ",");
        if (token != NULL) strcpy(name, token);

        token = strtok(NULL, ",");
        if (token != NULL) grade = atoi(token);

        token = strtok(NULL, ",");
        if (token != NULL) reward_points = (unsigned int)atoi(token);

        token = strtok(NULL, ",");
        if (token != NULL) last_month_total = (unsigned int)atoi(token);

        dinsert(head); // 새로운 노드를 리스트에 추가
        setCustomer(&(head->rlink->data), name, id, grade, reward_points, last_month_total);
    }

    fclose(file);
}

int main(void)
{
    char ch;
    char name[80];
    int id;
    DListNode* head = (DListNode*)malloc(sizeof(DListNode));
    init(head);

    // 파일에서 고객 정보 로드
    customersFromFile(head, "파일 경로");   //구분자 (,)
                                            //ID,이름,등급,적립포인트,전월실적

    while (1) {
        printf("ID 입력(종료하려면 0 입력) : ");
        scanf("%d", &id);
        if (id == 0) break;
        getchar();
        DListNode* duplicate = findCustomerID(head, id);
        if (duplicate != NULL) {
            printf("중복된 ID입니다. 다시 입력하세요.\n");
            continue; // 중복된 경우 다시 입력을 받습니다.
        }
        printf("이름 입력(종료하려면 end 입력) : ");
        scanf("%s", name);
        printf("--------------------ID추가 완료---------------------\n ");
        dinsert(head);
        initializeCustomer(&(head->rlink->data), name, id);
    }

    DListNode* current = NULL;
    current = head->rlink;
    printAllCustomers(head);
    getchar();
    do {
        printf("\n명령어를 입력하시오(<, >, q): ");
        ch = getchar();
        if (ch == '<') {
            current = current->llink;
            if (current == head)
                current = current->llink;
        }
        else if (ch == '>') {
            current = current->rlink;
            if (current == head)
                current = current->rlink;
        }
        printUser(current);
        getchar();
    } while (ch != 'q');

    printUser(current);
    printAllCustomers(head);

    // 동적 메모리 해제 코드
    DListNode* temp = head->rlink;
    while (temp != head) {
        DListNode* next = temp->rlink;
        free(temp);
        temp = next;
    }
    free(head);

    return 0;
}