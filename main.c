// "main.c"
// 카드사 회원 관리 프로그램

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
    char name[80];                 // 이름
    int id;                        // 아이디
    int grade;                     // 등급
    unsigned int asset;            // 보유자산
    unsigned int reward_points;    // 적립 포인트
    unsigned int last_month_total; // 전월 실적
} Customer;

// element 타입 설정
#define ELEMENT_TYPE
typedef struct Customer element;
#include "LinkedList.h" // 이중연결리스트 코드

// 회원 정보 초기화 함수
void initializeCustomer(Customer* customer, char* name, int id) {
    strcpy(customer->name, name);
    customer->id = id;
    customer->grade = FAMILY;
    customer->asset = 0;
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
void setCustomer(Customer* customer, char* name, int id, unsigned int asset,
    unsigned int reward_points, unsigned int last_month_total) {
    strcpy(customer->name, name);
    customer->id = id;
    customer->asset = asset;
    customer->reward_points = reward_points;
    customer->last_month_total = last_month_total;
    customer->grade = FAMILY;
    updateGrade(customer);
}

// 문자열 등급 리턴 함수
const char* getGrade(int grade) {
    switch (grade) {
    case FAMILY:
        return "FAMILY";
    case BEST:
        return "BEST";
    case GRAND:
        return "GRAND";
    case VIP:
        return "VIP";
    case VVIP:
        return "VVIP";
    default:
        return "UNKNOWN";
    }
}

// 회원 정보 출력 함수
void printUser(DListNode* current) {
    Customer* user = &(current->data);
    printf("%-15s: %d\n", "회원 아이디", user->id);
    printf("%-15s: %s\n", "회원 이름", user->name);
    printf("%-15s: %d\n", "보유 자산", user->asset);
    printf("%-15s: %s\n", "회원 등급", getGrade(user->grade));
    printf("%-15s: %d\n", "보유 포인트", user->reward_points);
}

// 모든 회원 출력
void printAllCustomers(DListNode* head) {
    DListNode* current = head->rlink;
    Customer* user = NULL;
    printf("%-10s %-20s %-10s %-15s %-10s\n", "ID", "이름", "자산", "적립 포인트", "회원등급");
    printf("--------------------------------------------------------------------\n");
    while (current != head) {
        user = &(current->data);
        printf("%-10d %-20s %-10d %-15d %-10s\n",
            user->id, user->name, user->asset,
            user->reward_points, getGrade(user->grade));
        current = current->rlink;
    }
}

// 회원 ID로 회원 정보를 탐색하는 함수
DListNode* findCustomerID(DListNode* head, int id) {
    DListNode* current = head->rlink;
    // 리스트 순회하며 탐색
    while (current != head) {
        if (current->data.id == id) {
            return current;
        }
        current = current->rlink;
    }
    return NULL;
}

// 파일에서 회원 정보 읽어오기
int customersFromFile(DListNode* head, const char* filename) {
    FILE* file = fopen(filename, "r");  // 파일 내용: ID,이름,보유자산,적립포인트,전월실적(정수,문자열,정수,정수,정수)
    if (file == NULL) {                 // 인코딩: ANSI
        perror("파일 열기 실패");
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char name[80];
        int id, grade;
        unsigned int asset, reward_points, last_month_total;

        // ','을 기준으로 구분
        char* token = strtok(line, ",");
        if (token != NULL)
            id = atoi(token);
        DListNode* duplicate = findCustomerID(head, id);
        if (duplicate != NULL) {
            continue; // 중복된 경우 삽입 X
        }

        token = strtok(NULL, ",");
        if (token != NULL)
            strcpy(name, token);

        token = strtok(NULL, ",");
        if (token != NULL)
            asset = (unsigned int)atoi(token);

        token = strtok(NULL, ",");
        if (token != NULL)
            reward_points = (unsigned int)atoi(token);

        token = strtok(NULL, ",");
        if (token != NULL)
            last_month_total = (unsigned int)atoi(token);

        dinsert(head); // 새로운 노드를 리스트에 추가
        setCustomer(&(head->rlink->data), name, id, asset, reward_points, last_month_total);
    }

    fclose(file);
    return 1;
}

// 회원 추가 함수
void inputCustomer(DListNode* head){
    int ch;
    char name[80];
    int id;
    unsigned int asset;
    char filename[100];
    DListNode* duplicate = NULL;

    printf("\n1. 회원 정보 직접 입력\n");
    printf("2. 파일에서 정보 입력\n");
    printf("메뉴 선택: ");
    scanf("%d", &ch);
    getchar(); // 버퍼 비우기
    switch (ch) {
    case 1:
        while (1) {
            // 회원 정보 직접 입력
            printf("\n아이디 입력(이전 메뉴: 0): ");
            scanf("%d", &id);
            if (id == 0)
                break;
            duplicate = findCustomerID(head, id);
            if (duplicate != NULL) {
                printf("중복된 ID입니다. 다시 입력하세요.\n");
                // 중복된 경우 다시 입력을 받습니다.
                continue;
            }
            printf("이름 입력: ");
            scanf("%s", name);
            printf("보유 자산 입력: ");
            scanf("%u", &asset);
            dinsert(head);
            setCustomer(&(head->rlink->data), name, id, asset, 0, 0);
            printf("\n--------------------ID추가 완료---------------------\n ");
        }
        break;
    case 2:
        // 파일 경로 입력 받기
        printf("\n파일 경로를 입력하세요: ");
        scanf("%s", filename);
        // 파일에서 회원 정보 로드
        if (!customersFromFile(head, filename))
            break; // 구분자 (,)
            // ID,이름,보유자산,적립포인트,전월실적
        else {
            printf("\n--------------------ID추가 완료---------------------\n ");
            break;
        }
    default:
        printf("잘못된 선택입니다. 다시 선택해 주세요.\n");
        break;
    }
}

// 회원 삭제 함수
void deleteCustomer(DListNode* head) {
    int id;
    DListNode* target = NULL;

    while (1) { // 반복 삭제
        printf("\n아이디 입력(이전 메뉴: 0): ");
        scanf("%d", &id);
        if (id == 0)
            break;
        //삭제할 회원을 리스트에서 찾기
        target = findCustomerID(head, id);
        if (target == NULL) {
            printf("삭제할 회원을 찾을 수 없습니다.\n");
            continue;
        }
        //찾으면 회원 정보 출력하고 리스트에서 삭제
        printUser(target);
        target->llink->rlink = target->rlink;
        target->rlink->llink = target->llink;
        free(target);
        printf("회원 (ID: %d)이(가) 삭제되었습니다.\n", id);
    }
}

void searchCustomer(DListNode* head) {
    int ch;
    int id;
    DListNode* customerNode = NULL;

    while (1) {
        printf("\n1. 모든 회원 정보 출력\n");
        printf("2. 회원 정보 출력\n");
        printf("메뉴 선택(이전 메뉴: 0): ");
        scanf("%d", &ch);
        getchar(); // 버퍼 비우기
        if (ch == 0)
            break;
        switch (ch) {
        case 1:
            printAllCustomers(head);
            break;
        case 2:
            while (1) {
                printf("\nID로 회원 검색 (이전 메뉴: 0) : ");
                scanf("%d", &id);
                if (id == 0)
                    break;
                customerNode = findCustomerID(head, id);
                if (customerNode == NULL)
                    printf("아이디가 없습니다.\n");
                else
                    printUser(customerNode);
            }
            break;
        default:
            printf("잘못된 선택입니다. 다시 선택해 주세요.\n");
            break;
        }
    }
}

// 전체 회원 수 계산 함수
int countCustomers(DListNode* head) {
    int count = 0;
    DListNode* current = head->rlink;
    // 리스트 순회하며 count 증가
    while (current != head) {
        count++;
        current = current->rlink;
    }
    return count;
}

// 상위 1% 회원 추출 출력 함수
void printTop1PercentCustomers(DListNode* head) {
    // 전체 회원 수를 계산하고 상위 1%의 회원 수를 결정합니다.
    int totalCustomers = countCustomers(head);
    int top1PercentCount = (totalCustomers * 0.01);
    if (top1PercentCount == 0) top1PercentCount = 1;

    // 힙 자료구조 정의: Customer 포인터를 저장하는 배열을 동적으로 할당합니다.
    Customer** heap = (Customer**)malloc(sizeof(Customer*) * totalCustomers);
    int heapSize = 0;

    // 이중 연결 리스트를 순회하며 각 노드를 힙에 삽입합니다.
    // 삽입 시 상향식으로 힙의 성질을 유지하여 최대 힙을 만듭니다.
    for (DListNode* current = head->rlink; current != head; current = current->rlink) {
        heap[heapSize] = &(current->data);
        int i = heapSize++;
        while (i > 0 && heap[i]->asset > heap[(i - 1) / 2]->asset) {
            Customer* temp = heap[i];
            heap[i] = heap[(i - 1) / 2];
            heap[(i - 1) / 2] = temp;
            i = (i - 1) / 2;
        }
    }

    // 상위 1% 회원을 출력합니다.
    printf("\n상위 1%% 회원 목록\n");
    printf("-------------------------------------------------------------------\n");
    printf("%-10s %-20s %-10s %-15s %-10s\n", "ID", "이름", "자산", "적립 포인트", "회원등급");
    printf("-------------------------------------------------------------------\n");

    for (int i = 0; i < top1PercentCount; i++) {
        if (heapSize == 0) break;

        // 가장 높은 등급의 회원을 출력합니다.
        Customer* topCustomer = heap[0];
        printf("%-10d %-20s %-10d %-15d %-10s\n",
            topCustomer->id, topCustomer->name, topCustomer->asset,
            topCustomer->reward_points, getGrade(topCustomer->grade));

        // 마지막 요소를 루트로 이동시킵니다.
        heap[0] = heap[--heapSize];
        int parent = 0;
        while (1) {
            int leftChild = 2 * parent + 1;
            int rightChild = 2 * parent + 2;
            int largest = parent;

            if (leftChild < heapSize && heap[leftChild]->asset > heap[largest]->asset) {
                largest = leftChild;
            }
            if (rightChild < heapSize && heap[rightChild]->asset > heap[largest]->asset) {
                largest = rightChild;
            }
            if (largest == parent) break;

            // 부모와 가장 큰 자식을 교환합니다.
            Customer* temp = heap[parent];
            heap[parent] = heap[largest];
            heap[largest] = temp;
            parent = largest;
        }
    }

    // 동적으로 할당한 힙 메모리를 해제합니다.
    free(heap);
}

int main(void) {
    int ch;

    DListNode* head = (DListNode*)malloc(sizeof(DListNode));
    init(head);
    do {
        printf("\n1. 회원 추가\n");
        printf("2. 회원 삭제\n");
        printf("3. 회원 검색\n");
        printf("4. 상위 1%% 회원 출력\n");
        printf("메뉴 선택 (종료: 0): ");
        scanf("%d", &ch);
        getchar(); // 버퍼 비우기

        switch (ch) {
        case 1:
            inputCustomer(head);    //회원 추가
            break;
        case 2:
            deleteCustomer(head);   //회원 삭제
            break;
        case 3:
            searchCustomer(head);   //회원 검색
            break;
        case 4:
            printTop1PercentCustomers(head); //상위 회원 출력
            break;
        case 0:
            printf("프로그램을 종료합니다.\n");
            break;
        default:
            printf("잘못된 선택입니다. 다시 선택해 주세요.\n");
            break;
        }
    } while (ch != 0);

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
