# worktime

## ��Ŀ����
* ����ʱ��ͳ�ƹ��ߡ�

## ��������
* �����У�`gcc -O3 -o out\wt.exe src\*.c `

## ��������
* ��������`t <p[number]> [title]`��`task <p[number]> [title]`
* �������`c [number]`��`complete [number]`
    * `number`�ɴ��򲻴�`#`
* ɾ������`d [number]`��`delete [number]`
    * `number`�ɴ��򲻴�`#`
* ���ӹ�ʱ��`h [number] [hour]<h> <title>`��`hour [number] [hour]<h> <title>`
    * `number`�ɴ��򲻴�`#`
    * `hour`��Ϊ���������`.5`С����С������Ϊ`5`��ʹ��С����
* ��ʾ����`l <options>`��`list <options>`
    * ���²�����ƴ��ʹ�ã�����˳���磺`ahtd10`
    * ��ʾ��������`a`
    * ���ع�ʱ��Ϣ��`h`
    * ��ʾ����������Ϣ��`t`
    * ָ�����ڣ�`d<days>`�����ѯ10�ŵģ�`d10`
* �ָ���ʷ��`r [number]`��`restore [number]`
    * `number`�ɴ��򲻴�`#`���Ὣ`.wt_histories`Ŀ¼����������ļ���ԭ��
* �ֶ����ݣ�`ac [suffix]`��`restore [suffix]`
    * `suffix`һ�������ڣ�����`1810`����2018��10��,���ڵ�ǰĿ¼��`.wt_backups`�д���`worktime.wt.1810`�ı����ļ�
* �鿴���ݣ�`ck [suffix] <options>`��`check [suffix]`
    * `suffix`һ�������ڣ�����`1810`����2018��10��
