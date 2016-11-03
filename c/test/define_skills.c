
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
/*宏函数：能获取到任何数组的大小，原理是将数组整体的大小除以一个元素的大小就能获得数组中元素的个数*/

#define init_waitqueue_head(q)
	do {						\
		static struct lock_class_key __key; \
							\
		__init_waitqueue_head((q), &__key); \
	}while(0)
/*do{ }while(0) 的函数体中有其他函数体的调用*/
Linux内核中的do{}while(0)
     在Linux内核中，经常会看到do{}while(0)这样的语句，刚开始会疑惑，认为do{}while(0)毫无意义，因为它只会执行一次，加不加do{}while(0)效果是完全一样的，其实do{}while(0)只要用于宏定义中。
     这里用以个简单点的宏来演示：
 #define SAFE_FREE(P)   do{free(p); p = NULL;}while(0)
     假设这里去掉do{}while(0)，即定义SAFE_FREE为：
 #define SAFE_FREE(P)   free(p); p = NULL;
     那么以下代码：
  if(NULL!=P)
    SAFE_FREE(P)
  else 
   ...// do something
    会被展开为：
  if(NULL!=P)
   free(p); p = NULL;
  else 
   ...// do something
    展开的代码中存在两个问题：
 （1）if分支后有两个语句，导致else分支没有对应的if，编译失败；
 （2）假设没有else分支，则SAFE_FREE中的第二个语句无论if条件是否为真都会被执行。
   将SAFE_FREE的第定义加上{}就可以解决上述问题了，即：
   #define SAFE_FREE(P)   {free(p); p = NULL;}
    这样，代码：
  if(NULL!=P)
   SAFE_FREE(P) 
  else 
   ...// do something
    会被展开为：
  if(NULL!=P)
   {free(p); p = NULL;}
  else 
   ...// do something
    但是，在C语言中，每个语句后面加个分号是一种约定俗成的习惯，那么，如下代码：
  if(NULL!=P)
   SAFE_FREE(P);
  else 
   ...// do something
   将被展开为：
  if(NULL!=P)
   {free(p); p = NULL;};
  else 
   ...// do something
    这样，else分支就又没有对应的if了，编译将无法通过。假设用了do{}while(0)，情况就不一样了，同样的代码就会被展开为：
 if(NULL!=P)
   {free(p); p = NULL;}
  else 
   ...// do something
   不会在出现编译问题。  do{}while(0)的使用完全是为了保证宏定义的使用者能无编译错误的使用宏，它不对其使用者做任何假设
	
#define min(x,y) ({ \
	typeof(x) _x = (x); \
	typeof(y) _y = (y); \
	(void) (&_x == &_y);   \
	_x < _y ? _x : _y;})

	
#define max(x,y) ({ \
	typeof(x) _x = (x); \
	typeof(y) _y = (y); \
	(void) (&_x == &_y);   \
	_x > _y ? _x : _y;})
typeof关键字是C语言中的一个新扩展。

typeof的参数可以是两种形式：表达式或类型。
下面是使用表达式的的例子：
    typeof(x[0](1)
这里假设x是一个函数指针数组，这样就可以得到这个函数返回值的类型了。
如果将typeof用于表达式，则该表达式不会执行。只会得到该表达式的类型。
以下示例声明了int类型的var变量，因为表达式foo()是int类型的。由于表达式不会被执行，所以不会调用foo函数。
   extern int foo();
   typeof(foo()) var;

下面是用类型作参数的例子：
   typeof(int *) a,b;
等价于：
   int *a,*b;

下面是两个等效声明，用于声明int类型的变量a。
   typeof(int) a; /*int类型*/
   typeof('b') a; /* GCC中这个表达式的类型是int(自动提升为int)，
                  注意typeof(char)和typeof('b')得到的不是一样的，这个用sizeof可以看出来*/


一般情况下用typeof就可以了，但是如果要于ISO C兼容的话，最好是用双下划线的形式：__typeof__。
typeof和typedef很像，事实上，只要能用typedef的地方就可以用typeof。

下面是另外一些例子：
把y定义成x指向的数据类型：
   typeof(*x) y;
把y定义成x指向数据类型的数组：
   typeof(*x) y[4];
把y定义成一个字符指针数组：
   typeof(typeof(char *)[4]) y;
这与下面的定义等价：
   char *y[4];

我们再换一种定义方式：
   #define pointer(T) typeof(T *)
   #define array(T,N) typeof(T [N])
   array (pointer(char),4) y;

"如果想把T定义成一个表达式的类型，则我们仅仅用typedef无法做到但可以通过typeof做到"
   typdef typeof(expr) T;

使用typeof的声明示例
以下示例用于声明指针和数组。为了进行对比，还给出了不带typeof的等效声明。
   typeof(int *) p1,p2; /* Declares two int pointers p1, p2 */
   int *p1, *p2;

   typeof(int) *p3,p4;/* Declares int pointer p3 and int p4 */
   int *p3, p4;

   typeof(int [10]) a1, a2;/* Declares two arrays of integers */

   int a1[10], a2[10];

使用typeof的声明限制
请注意，typeof构造中的类型名不能包含存储类说明符，如extern或static。不过允许包含类型限定符，如const或volatile。
例如，下列代码是无效的，因为它在typeof构造中声明了extern：
   typeof(extern int) a;

下列代码使用外部链接来声明标识符b是有效的，表示一个int类型的对象。下一个声明也是有效的，它声明了一个使用const限定符的char类型指针，表示指针p不能被修改。
   extern typeof(int) b;
   typeof(char * const) p = "a";

在宏声明中使用typeof
typeof构造的主要应用是用在宏定义中。可以使用typeof关键字来引用宏参数的类型。因此，在没有将类型名明确指定为宏实参的情况下，构造带有所需类型的对象是可能的。
下面是一个交换两个变量的值的宏定义：
   #define SWAP(a,b) {\
      typeof(a) _t=a;\
      a=b;\
      b=_t;}
这个宏可以交换所有基本数据类型的变量(整数，字符，结构等）
	
	
for(;;)//用法相当于while(1)
{
	
}
/*已知结构体内变量的地址，算出结构体的首地址*/
#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif
/*大结构体中含有多个子结构体，使用子结构体变量算出整个大结构体的首地址
 *非指针类型也可以用*/
#define container_of(ptr, type, member) ({			\
	const typeof(((type *)0)->member) * __mptr = (ptr);	\
	(type *)((char *)__mptr - offsetof(type, member)); })

offset宏讲解
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE*)0)->MEMBER)

对这个宏的讲解我们大致可以分为以下4步进行讲解：

1>( (TYPE *)0 )  0地址强制 "转换" 为 TYPE结构类型的指针；

2>((TYPE *)0)->MEMBER  访问TYPE结构中的MEMBER数据成员；

3>&( ( (TYPE *)0 )->MEMBER)取出TYPE结构中的数据成员MEMBER的地址；

4>(size_t)(&(((TYPE*)0)->MEMBER))结果转换为size_t类型。

宏offsetof的巧妙之处在于将0地址强制转换为 TYPE结构类型的指针，TYPE结构以内存空间首地址0作为起始地址，则成员地址自然为偏移地址。可能有的读者会想是不是非要用0呢？当然不是，我们仅仅是为了计算的简便。也可以使用是他的值，只是算出来的结果还要再减去该数值才是偏移地址。来看看如下的代码：

  #include<stdio.h>

  #defineoffsetof(TYPE, MEMBER) ((size_t) &((TYPE *)4)->MEMBER)

  struct test_struct {

          int num;

          char ch;

          float f1;

  };

 int main(void)

  {

    printf("offsetof (struct test_struct,num)=%d\n",offsetof(struct test_struct,num)-4);

    printf("offsetof (structtest_struct,ch) =%d\n",offsetof(struct test_struct,ch)-4);

    printf("offsetof (struct test_struct,f1)=%d\n",offsetof(struct test_struct,f1)-4);

          return 0;

  }

运行结果为：

jibo@jibo-VirtualBox:~/cv_work/work/list/offset $ ./main

offsetof (struct test_struct,num) =0

offsetof (struct test_struct,ch) =4

offsetof (struct test_struct,f1) =8

为了让大家加深印象，我们在代码中没有使用0，而是使用的4，所以在最终计算出的结果部分减去了一个4才是偏移地址，当然实际使用中我们都是用的是0。

二.举例体会offsetof宏的使用：
  #include<stdio.h>

  #defineoffsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

  struct test_struct {

          int num;

          char ch;

          float f1;

  };

 int main(void)

  {

      printf("offsetof(struct test_struct,num) =%d\n",offsetof(struct test_struct,num));

    printf("offsetof (structtest_struct,ch) =%d\n",offsetof(struct test_struct,ch));

    printf("offsetof (struct test_struct,f1)=%d\n",offsetof(struct test_struct,f1));

          return 0;

  }

执行结果为：

jibo@jibo-VirtualBox:~/cv_work/work/list/offset $ ./main

offsetof (struct test_struct,num) =0

offsetof (struct test_struct,ch) =4

offsetof (struct test_struct,f1) =8

本篇文章来源于 Linux公社网站(www.linuxidc.com)  原文链接：http://www.linuxidc.com/Linux/2013-01/78004.htm


#define __bio_for_each_segment(bvl, bio, i, start_idx)			\
	for (bvl = bio_iovec_idx((bio), (start_idx)), i = (start_idx);	\
	     i < (bio)->bi_vcnt;					\
	     bvl++, i++)

#define bio_for_each_segment(bvl, bio, i)				\
	__bio_for_each_segment(bvl, bio, i, (bio)->bi_idx)