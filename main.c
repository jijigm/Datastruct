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

// 회원 구조체
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

// 회원 정보 초기화 함수
void initializeCustomer(Customer* customer, char* name, int id) {
    strcpy(customer->name, name);
    customer->id = id;
    customer->grade = FAMILY;
    customer->reward_points = 0;
    customer->last_month_total = 0;
}

// 회원 등급 갱신
void updateGrade(Customer* customer) {
    if (customer->last_month_total < 300000) {
        customer->grade = FAMILY;
    }
    else if (customer->last_month_total < 700000) {
        customer->grade = BEST;
    }
    else if (customer->last_month_total < 5000000) {
        customer->grade = GRAND;
    }
    else if (customer->last_month_total < 30000000) {
        customer->grade = VIP;
    }
    else
        customer->grade = VVIP;
}

// 회원 정보 설정 함수
void setCustomer(Customer* customer, char* name, int id,
    unsigned int reward_points, unsigned int last_month_total) {
    strcpy(customer->name, name);
    customer->id = id;
    customer->reward_points = reward_points;
    customer->last_month_total = last_month_total;
    customer->grade = FAMILY;
    updateGrade(customer);
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
        printf("ID: %d, 이름: %s, 적립 포인트: %d, 회원등급: %s\n",
            user->id, user->name,
            user->reward_points, getGrade(user->grade));
        current = current->rlink;
    }
}

// 회원 ID로 회원 정보를 탐색하는 함수
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

// 파일에서 회원 정보 읽어오기
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
        DListNode* duplicate = findCustomerID(head, id);
        if (duplicate != NULL) {
            continue; // 중복된 경우 삽입 X
        }

        token = strtok(NULL, ",");
        if (token != NULL) strcpy(name, token);

        token = strtok(NULL, ",");
        if (token != NULL) reward_points = (unsigned int)atoi(token);

        token = strtok(NULL, ",");
        if (token != NULL) last_month_total = (unsigned int)atoi(token);

        dinsert(head); // 새로운 노드를 리스트에 추가
        setCustomer(&(head->rlink->data), name, id, reward_points, last_month_total);
    }

    fclose(file);
}
// 회원 추가 함수
void inputCustomer(DListNode* head){
    int ch;
    char name[80];
    int id;
    char filename[100];

    printf("1. 회원 정보 직접 입력\n");
    printf("2. 파일에서 정보 입력\n");
    printf("메뉴 선택: ");
    scanf("%d", &ch);
    getchar(); // 버퍼 비우기
    switch (ch) {
        case 1:
            // 회원 정보 직접 입력
            printf("이름 입력: ");
            scanf("%s", name);
            printf("아이디 입력: ");
            scanf("%d", &id);
            DListNode *duplicate = findCustomerID(head, id);
            if (duplicate != NULL)
            {
                printf("중복된 ID입니다. 다시 입력하세요.\n");
                // 중복된 경우 다시 입력을 받습니다.
                break;
            }
            initializeCustomer(&(head->rlink->data), name, id);
            dinsert(head);
            printf("--------------------ID추가 완료---------------------\n ");
            break;
        case 2:
            // 파일 경로 입력 받기
            printf("파일 경로를 입력하세요: ");
            scanf("%s", filename);
            // 파일에서 회원 정보 로드
            customersFromFile(head, filename); // 구분자 (,)
                                               // ID,이름,적립포인트,전월실적
            printf("--------------------ID추가 완료---------------------\n ");
            break;
        default:
            printf("잘못된 선택입니다.\n");
            break;
    }
}
// 회원 삭제 함수
void deleteCustomer(DListNode* head) {
    int id;
    printf("아이디 입력: ");
            scanf("%d", &id);
    DListNode* target = findCustomerID(head,id);
    if (target == NULL) {
        printf("삭제할 회원을 찾을 수 없습니다.\n");
        return;
    }

    target->llink->rlink = target->rlink;
    target->rlink->llink = target->llink;
    free(target);
    printf("회원 (ID: %d)이(가) 삭제되었습니다.\n", id);
}

int main(void)
{
    int ch;
    int id;
    DListNode* head = (DListNode*)malloc(sizeof(DListNode));
    init(head);

    do {
        printf("\n1. 회원 추가\n");
        printf("2. 회원 삭제\n");
        printf("메뉴 선택 (종료: 0): ");
        scanf("%d", &ch);
        getchar(); // 버퍼 비우기

        switch (ch) {
            case 1:
                inputCustomer(head);
                break;
            case 2:
                deleteCustomer(head);
                break;
            case 0:
                printf("프로그램을 종료합니다.\n");
                break;
            default:
                printf("잘못된 선택입니다. 다시 선택해 주세요.\n");
                break;
        }
    } while (ch != 0);

    DListNode *current = NULL;
    current = head->rlink;
    printAllCustomers(head);
    getchar();
    while (1){
        printf("\nID로 회원 검색 (종료하려면 0 입력) : ");
        scanf("%d", &id);
        if (id == 0)
            break;
        DListNode *customerNode = findCustomerID(head, id);
        if (customerNode == NULL){
            printf("아이디가 없습니다.\n");
            continue;
        }
        printUser(customerNode);
    }

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