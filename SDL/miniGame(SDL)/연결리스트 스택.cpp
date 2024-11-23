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

/*초기화 함수
void init(LinkedStackType *s) { */
void init(StackNode** top) {
	*top = NULL;
}

/*공백 상태 검출 함수
int is_empty(LinkedStackType *s) { */
int is_empty(StackNode* top) {
	// return (s->top == NULL);
	return (top == NULL);
}

/*포화 상태 검출 함수
int is_full(LinkedStackType *s) { */
int is_full(StackNode* top) {
	return 0;
}

/* 삽입 함수
void push(LinkedStackType *s, element item) { */
void push(StackNode** top, element item) {
	StackNode* temp = (StackNode*)malloc(sizeof(StackNode));
	temp->data = item;
	temp->link = *top;
	//s->top = temp;
	*top = temp;
}


/* 삭제 함수
element pop(LinkedStackType *s) { */
element pop(StackNode** top) {
	// if (is_empty(s)) {
	if (is_empty(*top)) {
		fprintf(stderr, "스택이 비어있음\n");
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

/*피크 함수
element peek(LinkedStackType *s) { */
element peek(StackNode* top) {
	//if (is_empty(s)) {
	if (is_empty(top)) {
		fprintf(stderr, "스택이 비어있음\n");
		exit(1);
	}
	else {
		//return s->top->data;
		return top->data;
	}
}

//출력 함수(원본 포인터 변경이 필요없기에 단일 포인터 사용)
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
