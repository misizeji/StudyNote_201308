#ifndef __GPRS_H__
#define __GPRS_H__


int gprs_msg_send(char *phone_num, char *phone_msg);	//phone_num是发送的手机号码 phone_msg是发送的信息内容（英文）

int gprs_login_num_send(char *phone_num);//发送验证码函数


#endif 