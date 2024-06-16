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
        printf("ID: %d, Name: %s, Reward Points: %d, Grade: %s\n",
            user->id, user->name,
            user->reward_points, getGrade(user->grade));

        current = current->rlink;
    }
}

int main(void)
{
    char ch;
    char name[80];
    int id;
    DListNode* head = (DListNode*)malloc(sizeof(DListNode));
    init(head);
    while (1) {
        printf("ID 입력(종료하려면 0 입력) : ");
        scanf("%d", &id);
        if (id == 0) break;
        getchar();
        printf("이름 입력(종료하려면 end 입력) : ");
        scanf("%s", name);
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