#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int data;
    struct TreeNode* left, * right;
} TreeNode;

typedef struct ThreadTree {
    int data;
    struct ThreadTree* left, * right;
    int leftThread, rightThread;
} ThreadTree;


// ===================================================================

TreeNode* createNode(int data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

ThreadTree* createThreadNode(int data) {
    ThreadTree* newNode = (ThreadTree*)malloc(sizeof(ThreadTree));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->leftThread = 1;
    newNode->rightThread = 1;
    return newNode;
}

// ===================================================================

TreeNode* insert(TreeNode* node, int data) {
    if (node == NULL) return createNode(data);

    if (data < node->data)
        node->left = insert(node->left, data);
    else if (data > node->data)
        node->right = insert(node->right, data);

    return node;
}

ThreadTree* insertThread(ThreadTree* root, int data) {
    ThreadTree* newNode = createThreadNode(data);

    if (root == NULL)
        return newNode;

    ThreadTree* parent = NULL;
    ThreadTree* current = root;

    while (current != NULL) {
        parent = current;

        if (data < current->data) {
            if (current->leftThread == 0)
                current = current->left;
            else
                break;
        }
        else {
            if (current->rightThread == 0)
                current = current->right;
            else
                break;
        }
    }

    if (data < parent->data) {
        newNode->left = parent->left;
        newNode->right = parent;
        parent->leftThread = 0;
        parent->left = newNode;
    }
    else {
        newNode->right = parent->right;
        newNode->left = parent;
        parent->rightThread = 0;
        parent->right = newNode;
    }

    return root;
}

// ===================================================================

TreeNode* GenerateBinaryTree(int inputData[]) {
    TreeNode* root = NULL;
    int i = 0;

    while (1) {
        if (i >= 15)
            break;

        root = insert(root, inputData[i]);
        i++;
    }

    return root;
}

ThreadTree* GenerateThreadTree(int inputData[]) {
    ThreadTree* root = NULL;
    int i = 0;

    while (i < 15) {
        root = insertThread(root, inputData[i]);
        i++;
    }

    return root;
}

// ===================================================================

typedef struct Stack {
    TreeNode* nodes[100];
    int top;
} Stack;

void initStack(Stack* s) {
    s->top = -1;
}

int isEmpty(Stack* s) {
    return s->top == -1;
}

void push(Stack* s, TreeNode* node) {
    s->nodes[++(s->top)] = node;
}

TreeNode* pop(Stack* s) {
    return s->nodes[(s->top)--];
}

// ===================================================================

void BinaryTreeInOrder(TreeNode* root) {
    Stack s;
    initStack(&s);
    TreeNode* curr = root;

    while (curr != NULL || !isEmpty(&s)) {
        while (curr != NULL) {
            push(&s, curr);
            curr = curr->left;
        }

        curr = pop(&s);
        printf("%d ", curr->data);

        curr = curr->right;
    }

    printf("\n");
}

void ThreadTreeInOrder(ThreadTree* root) {
    if (root == NULL) return;

    ThreadTree* current = root;

    while (current->leftThread == 0)
        current = current->left;

    while (current != NULL) {
        printf("%d ", current->data);

        if (current->rightThread == 1) {
            current = current->right;
        }
        else {
            current = current->right;
            while (current != NULL && current->leftThread == 0) {
                current = current->left;
            }
        }
    }
    printf("\n");
}

// ===================================================================

int main() {
    int inputData[] = { 4, 1, 9, 13, 15, 3, 6, 14, 7, 10, 12, 2, 5, 8, 11 };
    // 트리 생성. *root에 트리를 생성해 반환
    TreeNode* root = GenerateBinaryTree(inputData);
    BinaryTreeInOrder(root);

    ThreadTree* troot = GenerateThreadTree(inputData);
    ThreadTreeInOrder(troot);

    free(root);
    free(troot);

    return 0;
}