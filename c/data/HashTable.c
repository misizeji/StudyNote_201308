#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define HASHSIZE 32	//32,50,101

//待存入表格数据
char *key_input[] = {
		"auto",   "break",  "case",     "char",   "const",    "continue", "default",  
		"do",
		"double", "else",   "enum",     "extern", "float",    "for",      "goto",     
		"if",
		"int",    "long",   "register", "return", "short",    "signed",   "sizeof",   
		"static",
		"struct", "switch", "typedef",  "union",  "unsigned", "void",     "volatile",
		"while"
};
char *key_help_temp[] = {
		"auto function",
		"break function",
		"case function",
		"char function",
		"const function",
		"continue function",
		"default function",  
		"do function",
		"double function",
		"else function",
		"enum function",
		"extern function",
		"float function",
		"for function",
		"goto function",     
		"if function",
		"int function",
		"long function",
		"register function",
		"return function",
		"short function",
		"signed function",
		"sizeof function",   
		"static function",
		"struct function",
		"switch function",
		"typedef function",
		"union function",
		"unsigned function",
		"void function",
		"volatile function",
		"while function"
};

//=========================================================================
//函数：unsigned int hash(char *key_input_temp)
//功能：哈希函数，从数据中抽出某个成员用于哈希值的计算
//参数：
//		key_input_temp:保存数组第0个元素的地址
//返回：通过hash函数得到下标
//维护：2013/05/07  sunplusedu  V6.5.0
//=========================================================================
unsigned int hash(char *key_input_temp)
{
	unsigned int hashval;
	char *s_temp = key_input_temp;
	int i = 0;

	for (hashval = 0; *key_input_temp != '\0'; key_input_temp++)	//第一个条件只是初始条件
	{
		hashval = *key_input_temp + 31 * hashval;
	}
	return (hashval % HASHSIZE);//返回下标(0-31)	
}
//=========================================================================
//函数：unsigned int conflict_deal(unsigned int pos_hash_temp,char val_flag[][20],char *val_flag_data)
//功能：进行冲突判断，以及冲突解决
//参数：
//		pos_hash_temp:哈希函数得得到的下标值
//		val_flag[][20]:数据存放标志数组
//		val_flag_data:冲突的标志，即什么样的数据才代表着冲突
//					  当存放数据时，为""
//					  当获取数据时，为相应的关键字，用来代表这是否就是那么想要的数据的下标
//返回：冲突处理过后的真正的下标
//维护：2013/05/07  sunplusedu  V6.5.0
//=========================================================================
unsigned int conflict_deal(unsigned int pos_hash_temp,char val_flag[][20],char *val_flag_data)
{
	int i=0;
	while( strcmp( val_flag[pos_hash_temp],val_flag_data ) != 0)
	{
		//printf("int conflict_deal function;*%s*,*%s*\n",val_flag[pos_hash_temp],val_flag_data);
		i++;
		pos_hash_temp = (pos_hash_temp + i)%HASHSIZE;	//冲突处理，存储单元(下标)偏移
	}
	return pos_hash_temp;
}

//=========================================================================
//函数：int main(int argc,char **argv) 
//功能：输入一个C语言关键字，快速提示这个关键字的功能及用法
//参数：无
//返回：无
//维护：2013/05/07  sunplusedu  V6.5.0
//=========================================================================
int main(int argc,char **argv) 
{
	char key_help[HASHSIZE][100]={""};	//关键字说明的存储空间
	char val_flag[HASHSIZE][20]={""};	//标志已占用存储单元
	int i, key_size;					//i：循环使用，key_size：关键字的个数
	unsigned int pos_hash,pos_conflict;	//pos_hash:通过hash计算出的下标，pos_confilct：冲突解决后的最终下标

	key_size = sizeof(key_input) / sizeof(key_input[0]);//计算关键字个数
	
	printf("\n==================开始存放数据=================\n");
	for(i = 0;i < key_size; i++)
	{
		pos_hash = hash(key_input[i]);						//调用“哈希、散列函数”得到坐标
		pos_conflict = conflict_deal(pos_hash,val_flag,"");	//冲突检测以及冲突处理函数，得到冲突解决后的最终下标
		
		strcpy( val_flag[pos_conflict] , key_input[i] );	//进行相应的标记，“这个位置已经有数据进行存放”
		strcpy( key_help[pos_conflict] , key_help_temp[i] );//按照坐标存放相应的数据到相应的位置
		
		printf("key_input=%s,pos_conflict=%d,val_falg[%d]=%s,----->%s\n",key_input[i],pos_conflict,pos_conflict,val_flag[pos_conflict],key_help[pos_conflict]);
	}
	
	printf("\n==================开始获取数据=================\n");
	// for(i=key_size-1; i >= 0; i--)//摸拟关键字输入，目的：检测能否根据哈希表得到相应的数据
	for(i=0; i < key_size; i++)//摸拟关键字输入，目的：检测能否根据哈希表得到相应的数据
	{
		pos_hash = hash(key_input[i]);
		pos_conflict = conflict_deal(pos_hash,val_flag,key_input[i]);
		printf("key_input=%s,pos_conflict=%d,val_falg[%d]=%s,----->%s\n",key_input[i],pos_conflict,pos_conflict,val_flag[pos_conflict],key_help[pos_conflict]);
	}
	return 0;
}
