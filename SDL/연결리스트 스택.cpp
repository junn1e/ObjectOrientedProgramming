#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef int element;
typedef struct StackNode {
	element data;
	struct StackNode* link;
} StackNode;

/*typedef struct {
	StackNode *top;
} LinkedStackType;*/

/*�ʱ�ȭ �Լ�
void init(LinkedStackType *s) { */
void init(StackNode** top) {
	*top = NULL;
}

/*���� ���� ���� �Լ�
int is_empty(LinkedStackType *s) { */
int is_empty(StackNode* top) {
	// return (s->top == NULL);
	return (top == NULL);
}

/*��ȭ ���� ���� �Լ�
int is_full(LinkedStackType *s) { */
int is_full(StackNode* top) {
	return 0;
}

/* ���� �Լ�
void push(LinkedStackType *s, element item) { */
void push(StackNode** top, element item) {
	StackNode* temp = (StackNode*)malloc(sizeof(StackNode));
	temp->data = item;
	temp->link = *top;
	//s->top = temp;
	*top = temp;
}


/* ���� �Լ�
element pop(LinkedStackType *s) { */
element pop(StackNode** top) {
	// if (is_empty(s)) {
	if (is_empty(*top)) {
		fprintf(stderr, "������ �������\n");
		exit(1);
	}
	else {
		//StackNode *temp = s->top;
		StackNode* temp = *top;
		int data = temp->data;
		//s->top = s->top->link;
		*top = (*top)->link;
		free(temp);
		return data;
	}
}

/*��ũ �Լ�
element peek(LinkedStackType *s) { */
element peek(StackNode* top) {
	//if (is_empty(s)) {
	if (is_empty(top)) {
		fprintf(stderr, "������ �������\n");
		exit(1);
	}
	else {
		//return s->top->data;
		return top->data;
	}
}

//��� �Լ�(���� ������ ������ �ʿ���⿡ ���� ������ ���)
void print_stack(StackNode* top) {
	// for (StackNode *p = s->top; p != NULL; p = p->link) {
	for (StackNode* p = top; p != NULL; p = p->link) {
		printf("%d->", p->data);
	}
	printf("NULL \n");
}



int main(void)
{
	// LinkedStackType s;
	StackNode* s;
	init(&s);
	push(&s, 1); print_stack(s);
	push(&s, 2); print_stack(s);
	push(&s, 3); print_stack(s);
	pop(&s); print_stack(s);
	pop(&s); print_stack(s);
	pop(&s); print_stack(s);
	return 0;
}
