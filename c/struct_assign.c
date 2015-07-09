//struct assign is actually memory copy!

#include <stdio.h>
#include <stdlib.h>

struct B {
	int *pi;
	char s[16];
};

struct A{
	int i;
	float f;
	struct B b;
};

int main(void)
{
	int *p=(int *)malloc(15*sizeof(int));
	struct A str1={5,5.4,{p,"rickyzhang"}};
	struct A str2=str1;
	struct A str3={
		.i=6,
		.f=7.6,
		.b={
			.pi=p,
			"hello" //.s="hello" is something wrong
		}
	};
	printf("p=%p\n",p);
	printf("str1:.i=%d,.f=%f,.b.pi=%p,.b.s=%s\n",str1.i,str1.f,str1.b.pi,str1.b.s);
	printf("str2:.i=%d,.f=%f,.b.pi=%p,.b.s=%s\n",str2.i,str2.f,str2.b.pi,str2.b.s);
	free(p);
	return 0;
}
