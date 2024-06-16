#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ���
#define FAMILY 1
#define BEST 2
#define GRAND 3
#define VIP 4
#define VVIP 5

// �� ����ü
typedef struct Customer {
    char name[80];  //�̸�
    int id;         //���̵�
    int grade;      //���
    unsigned int reward_points;     //���� ����Ʈ
    unsigned int last_month_total;  //���� ����
}Customer;

// element Ÿ�� ����
#define ELEMENT_TYPE
typedef struct Customer element;
#include "LinkedList.h" // ���߿��Ḯ��Ʈ �ڵ�

// �� ���� �ʱ�ȭ �Լ�
void initializeCustomer(Customer* customer, char* name, int id) {
    strcpy(customer->name, name);
    customer->id = id;
    customer->grade = FAMILY;
    customer->reward_points = 0;
    customer->last_month_total = 0;
}

// ����Ʈ ���� �Լ�
void getRewardPoints(Customer* customer, int payment_amount) {
    int grade = customer->grade;
    switch (grade) {
    case FAMILY:
        customer->reward_points += (int)(payment_amount * 0.01);    //unsigned int���� daouble�� ���� ���� (int)ĳ����
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

// ���ڿ� ��� ���� �Լ�
const char* getGrade(int grade) {
    switch (grade) {
    case FAMILY: return "FAMILY";
    case BEST: return "BEST";
    case GRAND: return "GRAND";
    case VIP: return "VIP";
    case VVIP: return "VVIP";
    }
}
// ȸ�� ���� ��� �Լ�
void printUser(DListNode* current) {
    Customer* user = &(current->data);
    printf("ȸ�� ���̵� : %d\n", user->id);
    printf("ȸ�� �̸� : %s\n", user->name);
    printf("ȸ�� ��� : %s\n", getGrade(user->grade));
    printf("���� ����Ʈ : %d\n", user->reward_points);
}

// ��� ȸ�� ���
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
        printf("ID �Է�(�����Ϸ��� 0 �Է�) : ");
        scanf("%d", &id);
        if (id == 0) break;
        getchar();
        printf("�̸� �Է�(�����Ϸ��� end �Է�) : ");
        scanf("%s", name);
        dinsert(head);
        initializeCustomer(&(head->rlink->data), name, id);
    }

    DListNode* current = NULL;
    current = head->rlink;
    printAllCustomers(head);
    getchar();
    do {
        printf("\n��ɾ �Է��Ͻÿ�(<, >, q): ");
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

    // ���� �޸� ���� �ڵ�
    DListNode* temp = head->rlink;
    while (temp != head) {
        DListNode* next = temp->rlink;
        free(temp);
        temp = next;
    }
    free(head);

    return 0;
}