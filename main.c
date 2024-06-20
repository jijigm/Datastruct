// "main.c"
// ī��� ȸ�� ���� ���α׷�

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ���
#define FAMILY 1
#define BEST 2
#define GRAND 3
#define VIP 4
#define VVIP 5

// ȸ�� ����ü
typedef struct Customer {
    char name[80];                 // �̸�
    int id;                        // ���̵�
    int grade;                     // ���
    unsigned int asset;            // �����ڻ�
    unsigned int reward_points;    // ���� ����Ʈ
    unsigned int last_month_total; // ���� ����
} Customer;

// element Ÿ�� ����
#define ELEMENT_TYPE
typedef struct Customer element;
#include "LinkedList.h" // ���߿��Ḯ��Ʈ �ڵ�

// ȸ�� ���� �ʱ�ȭ �Լ�
void initializeCustomer(Customer* customer, char* name, int id) {
    strcpy(customer->name, name);
    customer->id = id;
    customer->grade = FAMILY;
    customer->asset = 0;
    customer->reward_points = 0;
    customer->last_month_total = 0;
}

// ȸ�� ��� ����
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

// ȸ�� ���� ���� �Լ�
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

// ���ڿ� ��� ���� �Լ�
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

// ȸ�� ���� ��� �Լ�
void printUser(DListNode* current) {
    Customer* user = &(current->data);
    printf("%-15s: %d\n", "ȸ�� ���̵�", user->id);
    printf("%-15s: %s\n", "ȸ�� �̸�", user->name);
    printf("%-15s: %d\n", "���� �ڻ�", user->asset);
    printf("%-15s: %s\n", "ȸ�� ���", getGrade(user->grade));
    printf("%-15s: %d\n", "���� ����Ʈ", user->reward_points);
}

// ��� ȸ�� ���
void printAllCustomers(DListNode* head) {
    DListNode* current = head->rlink;
    Customer* user = NULL;
    printf("%-10s %-20s %-10s %-15s %-10s\n", "ID", "�̸�", "�ڻ�", "���� ����Ʈ", "ȸ�����");
    printf("--------------------------------------------------------------------\n");
    while (current != head) {
        user = &(current->data);
        printf("%-10d %-20s %-10d %-15d %-10s\n",
            user->id, user->name, user->asset,
            user->reward_points, getGrade(user->grade));
        current = current->rlink;
    }
}

// ȸ�� ID�� ȸ�� ������ Ž���ϴ� �Լ�
DListNode* findCustomerID(DListNode* head, int id) {
    DListNode* current = head->rlink;
    // ����Ʈ ��ȸ�ϸ� Ž��
    while (current != head) {
        if (current->data.id == id) {
            return current;
        }
        current = current->rlink;
    }
    return NULL;
}

// ���Ͽ��� ȸ�� ���� �о����
int customersFromFile(DListNode* head, const char* filename) {
    FILE* file = fopen(filename, "r");  // ���� ����: ID,�̸�,�����ڻ�,��������Ʈ,��������(����,���ڿ�,����,����,����)
    if (file == NULL) {                 // ���ڵ�: ANSI
        perror("���� ���� ����");
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char name[80];
        int id, grade;
        unsigned int asset, reward_points, last_month_total;

        // ','�� �������� ����
        char* token = strtok(line, ",");
        if (token != NULL)
            id = atoi(token);
        DListNode* duplicate = findCustomerID(head, id);
        if (duplicate != NULL) {
            continue; // �ߺ��� ��� ���� X
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

        dinsert(head); // ���ο� ��带 ����Ʈ�� �߰�
        setCustomer(&(head->rlink->data), name, id, asset, reward_points, last_month_total);
    }

    fclose(file);
    return 1;
}

// ȸ�� �߰� �Լ�
void inputCustomer(DListNode* head){
    int ch;
    char name[80];
    int id;
    unsigned int asset;
    char filename[100];
    DListNode* duplicate = NULL;

    printf("\n1. ȸ�� ���� ���� �Է�\n");
    printf("2. ���Ͽ��� ���� �Է�\n");
    printf("�޴� ����: ");
    scanf("%d", &ch);
    getchar(); // ���� ����
    switch (ch) {
    case 1:
        while (1) {
            // ȸ�� ���� ���� �Է�
            printf("\n���̵� �Է�(���� �޴�: 0): ");
            scanf("%d", &id);
            if (id == 0)
                break;
            duplicate = findCustomerID(head, id);
            if (duplicate != NULL) {
                printf("�ߺ��� ID�Դϴ�. �ٽ� �Է��ϼ���.\n");
                // �ߺ��� ��� �ٽ� �Է��� �޽��ϴ�.
                continue;
            }
            printf("�̸� �Է�: ");
            scanf("%s", name);
            printf("���� �ڻ� �Է�: ");
            scanf("%u", &asset);
            dinsert(head);
            setCustomer(&(head->rlink->data), name, id, asset, 0, 0);
            printf("\n--------------------ID�߰� �Ϸ�---------------------\n ");
        }
        break;
    case 2:
        // ���� ��� �Է� �ޱ�
        printf("\n���� ��θ� �Է��ϼ���: ");
        scanf("%s", filename);
        // ���Ͽ��� ȸ�� ���� �ε�
        if (!customersFromFile(head, filename))
            break; // ������ (,)
            // ID,�̸�,�����ڻ�,��������Ʈ,��������
        else {
            printf("\n--------------------ID�߰� �Ϸ�---------------------\n ");
            break;
        }
    default:
        printf("�߸��� �����Դϴ�. �ٽ� ������ �ּ���.\n");
        break;
    }
}

// ȸ�� ���� �Լ�
void deleteCustomer(DListNode* head) {
    int id;
    DListNode* target = NULL;

    while (1) { // �ݺ� ����
        printf("\n���̵� �Է�(���� �޴�: 0): ");
        scanf("%d", &id);
        if (id == 0)
            break;
        //������ ȸ���� ����Ʈ���� ã��
        target = findCustomerID(head, id);
        if (target == NULL) {
            printf("������ ȸ���� ã�� �� �����ϴ�.\n");
            continue;
        }
        //ã���� ȸ�� ���� ����ϰ� ����Ʈ���� ����
        printUser(target);
        target->llink->rlink = target->rlink;
        target->rlink->llink = target->llink;
        free(target);
        printf("ȸ�� (ID: %d)��(��) �����Ǿ����ϴ�.\n", id);
    }
}

void searchCustomer(DListNode* head) {
    int ch;
    int id;
    DListNode* customerNode = NULL;

    while (1) {
        printf("\n1. ��� ȸ�� ���� ���\n");
        printf("2. ȸ�� ���� ���\n");
        printf("�޴� ����(���� �޴�: 0): ");
        scanf("%d", &ch);
        getchar(); // ���� ����
        if (ch == 0)
            break;
        switch (ch) {
        case 1:
            printAllCustomers(head);
            break;
        case 2:
            while (1) {
                printf("\nID�� ȸ�� �˻� (���� �޴�: 0) : ");
                scanf("%d", &id);
                if (id == 0)
                    break;
                customerNode = findCustomerID(head, id);
                if (customerNode == NULL)
                    printf("���̵� �����ϴ�.\n");
                else
                    printUser(customerNode);
            }
            break;
        default:
            printf("�߸��� �����Դϴ�. �ٽ� ������ �ּ���.\n");
            break;
        }
    }
}

// ��ü ȸ�� �� ��� �Լ�
int countCustomers(DListNode* head) {
    int count = 0;
    DListNode* current = head->rlink;
    // ����Ʈ ��ȸ�ϸ� count ����
    while (current != head) {
        count++;
        current = current->rlink;
    }
    return count;
}

// ���� 1% ȸ�� ���� ��� �Լ�
void printTop1PercentCustomers(DListNode* head) {
    // ��ü ȸ�� ���� ����ϰ� ���� 1%�� ȸ�� ���� �����մϴ�.
    int totalCustomers = countCustomers(head);
    int top1PercentCount = (totalCustomers * 0.01);
    if (top1PercentCount == 0) top1PercentCount = 1;

    // �� �ڷᱸ�� ����: Customer �����͸� �����ϴ� �迭�� �������� �Ҵ��մϴ�.
    Customer** heap = (Customer**)malloc(sizeof(Customer*) * totalCustomers);
    int heapSize = 0;

    // ���� ���� ����Ʈ�� ��ȸ�ϸ� �� ��带 ���� �����մϴ�.
    // ���� �� ��������� ���� ������ �����Ͽ� �ִ� ���� ����ϴ�.
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

    // ���� 1% ȸ���� ����մϴ�.
    printf("\n���� 1%% ȸ�� ���\n");
    printf("-------------------------------------------------------------------\n");
    printf("%-10s %-20s %-10s %-15s %-10s\n", "ID", "�̸�", "�ڻ�", "���� ����Ʈ", "ȸ�����");
    printf("-------------------------------------------------------------------\n");

    for (int i = 0; i < top1PercentCount; i++) {
        if (heapSize == 0) break;

        // ���� ���� ����� ȸ���� ����մϴ�.
        Customer* topCustomer = heap[0];
        printf("%-10d %-20s %-10d %-15d %-10s\n",
            topCustomer->id, topCustomer->name, topCustomer->asset,
            topCustomer->reward_points, getGrade(topCustomer->grade));

        // ������ ��Ҹ� ��Ʈ�� �̵���ŵ�ϴ�.
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

            // �θ�� ���� ū �ڽ��� ��ȯ�մϴ�.
            Customer* temp = heap[parent];
            heap[parent] = heap[largest];
            heap[largest] = temp;
            parent = largest;
        }
    }

    // �������� �Ҵ��� �� �޸𸮸� �����մϴ�.
    free(heap);
}

int main(void) {
    int ch;

    DListNode* head = (DListNode*)malloc(sizeof(DListNode));
    init(head);
    do {
        printf("\n1. ȸ�� �߰�\n");
        printf("2. ȸ�� ����\n");
        printf("3. ȸ�� �˻�\n");
        printf("4. ���� 1%% ȸ�� ���\n");
        printf("�޴� ���� (����: 0): ");
        scanf("%d", &ch);
        getchar(); // ���� ����

        switch (ch) {
        case 1:
            inputCustomer(head);    //ȸ�� �߰�
            break;
        case 2:
            deleteCustomer(head);   //ȸ�� ����
            break;
        case 3:
            searchCustomer(head);   //ȸ�� �˻�
            break;
        case 4:
            printTop1PercentCustomers(head); //���� ȸ�� ���
            break;
        case 0:
            printf("���α׷��� �����մϴ�.\n");
            break;
        default:
            printf("�߸��� �����Դϴ�. �ٽ� ������ �ּ���.\n");
            break;
        }
    } while (ch != 0);

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
