#include <string.h>
#include <math.h>
#include "CodeExchange.h"

//Var
struct templat NONE= {0,"  ",0};
struct templat PLAYER= {1,"��",0};
struct templat AI= {2,"��"};
const int count_func=9;//used:entry_setting;setting_put_tips
int background_colour=0;
int font_colour=7;
int reset=0;
int ischangefirst=0;
enum change_modes {background,font};

const char* MCE[]=
{
    "����������(Format:x,y;���� 4,4 ����ѡ��ҳ��):x,y\b\b\b",
    "��������� 1 �� %d ������� 4,4 (ѡ��ָ��)!\n",
    "�밴��ʽ���룡",
    "�˴���������!"
};

//extern const char * PLAYER;
//extern const char * AI;
extern int getch(void);


void show_checkboard()
{
    int i;
    printf("   1   2    3\n");
    for(i=0; i<base; i++)
    {
        printf("%d %s | %s | %s \n",i+1,(!check_board[i][0])?NONE.piece:(check_board[i][0]==1)?PLAYER.piece:AI.piece,(!check_board[i][1])?NONE.piece:(check_board[i][1]==1?PLAYER.piece:AI.piece),(!check_board[i][2])?NONE.piece:(check_board[i][2]==1?PLAYER.piece:AI.piece));
        if(i!=base-1)
            printf("  -------------\n");
    }
}

void clean_screen(void)
{
    system("cls");
}

void user_input(void)
{
    int x,y;
    char temp;
    while(1)
    {

        if(reset)
        {
            reset_checkboard();
            show_default(1,0);
            reset=0;
            continue;
        }

        printf(MCE[0]);

        if(scanf("%d%c%d",&x,&temp,&y)!=3)
        {
            user_input_err();
            puts(MCE[2]);
            continue;
        }

        if(x>base+1 || y>base+1 ||x<1||y<1)
        {
            user_input_err();
            printf(MCE[1],base);
            continue;
        }
        if(x==4&&y==4)
        {
            entry_setting();
            continue;
        }
        else if ((x==4&&y!=4)||(x!=4&&y==4))
        {
            user_input_err();
            printf(MCE[1],base);
            continue;
        }
        if(check_board[x-1][y-1]!=NONE.num)
        {
            user_input_err();
            puts(MCE[3]);
            continue;
        }
        clean_buffer();
        refresh_checkboard(x-1,y-1,PLAYER.num);
        if(check_win())
        {
            reset_checkboard();
            return;
        }
        if(!response_advantages())
            response_danger();
        if(check_win())
        {
            reset_checkboard();
            return;
        }
    }
}


void refresh_checkboard(int x,int y,int value)
{
    check_board[x][y]=value;
    show_default(1,0);
}

void user_input_err(void)
{
    show_default(0,1);
}

int getchr(void)
{
    return putchar(getch());
}

void entry_setting(void)
{
    char input;
    int change_piece_choose;
    clean_screen();
    while(1)
    {
        setting_put_tips(0);
        input=getchr();
        input-='0';
        if(input<1||input>count_func)
        {
            setting_put_tips(1);
            continue;
        }
        else
            break;
    }
    switch(input)
    {
    case 1://���¿�ʼ
        clean_buffer();
        reset_checkboard();
        break;

    case 2://���üƷ�
        AI.points=PLAYER.points=NONE.points=0;
        show_default(1,0);
        break;

    case 3://��������
        putchar('\n');
        setting_put_tips(2);
        printf("��ѡ��:_\b");
        while(!scanf("%d",&change_piece_choose))
        {
            clean_buffer();
            setting_put_tips(1);
            setting_put_tips(2);
            printf("��ѡ��:_\b");
        }
        clean_buffer();
        if(change_piece_choose==3)
            show_default(1,0);
        else
            change_piece(change_piece_choose==1?PLAYER.num:(change_piece_choose==2?AI.num:NONE.num));
        break;
    case 8://����
        show_default(1,1);
        break;
    case 4://������ɫ
        change_colour();
        break;
    case 9://�˳�
        exit(0);
        break;
    case 7://����
        show_about();
        break;
    case 6://ʹ��˵��
        show_help();
        break;
    case 5:
        change_first();
        break;
    }

}

void setting_put_tips(int code)
{
    int i;
    const char* tips[]=
    {
        "���¿�ʼ",
        "���üƷ�",
        "��������",
        "���ı�����������ɫ",
        "�������·�",
        "ʹ��˵��",
        "���ڳ���",
        "������Ϸ",
        "�˳���Ϸ"
    };
    const char change_piece_tip[]="1.��� 2.����� 3.����";
    switch(code)
    {
    case 0:
        for(i=0; i<count_func; i++)
            printf("%d. %s   ",i+1,tips[i]);
        printf("\n��ѡ��:_\b");
        break;
    case 1:
        clean_screen();
        putchar('\a');
        puts("��������밴��ʾ���룡");
        break;
    case 2:
        clean_screen();
        puts(change_piece_tip);
        break;

    }

}

void reset_checkboard(void)
{
    int i,j;
    for(i=0; i<base; i++)
        for(j=0; j<base; j++)
            check_board[i][j]=0;
    if(first==AI.num)
        if(!response_advantages())
            response_danger();
    show_default(1,0);
}

void change_piece(int code)
{
    char input[3];

    clean_screen();
    printf("\n����ǰ��%s\n���ĺ�_\b",code==1?PLAYER.piece:(code==2?AI.piece:NONE.piece));
    scanf("%s",input);
    clean_buffer();
    if(input[0]=='\n')
        goto label_end;
    code=(code==1?PLAYER.num:(code==2?AI.num:NONE.num));
    switch(code)
    {
    case 1:
        memset(PLAYER.piece,' ',sizeof(char)*3);
        PLAYER.piece[0]=input[0];
        if(input[1]!='\0')
            PLAYER.piece[1]=input[1];
        PLAYER.piece[2]='\0';
        break;
    case 2:
        memset(AI.piece,' ',sizeof(char)*3);
        AI.piece[0]=input[0];
        if(input[1]!='\0')
            AI.piece[1]=input[1];
        AI.piece[2]='\0';
        break;
    case 3:
        show_default(1,0);
        break;
    }

label_end:
    show_default(1,0);

}

void show_default(int isnewline,int iscleanbuffer)
{
    if(iscleanbuffer)
        clean_buffer();
    clean_screen();
    show_checkboard();
    show_points();
    if(isnewline)
        putchar('\n');
}

void change_colour(void)
{
    int input;
    char cmd[10];
    char unit,ten;

    clean_screen();
    printf("1.���� 2.����\n��ѡ��:_\b");
    while(!scanf("%d",&input)||input>2||input<1)
    {
        setting_put_tips(1);
        clean_buffer();
        printf("1.���� 2.����\n��ѡ��:_\b");
    }
    clean_screen();
    if(input==1)
        show_colors(font);
    else
        show_colors(background);
    strcpy(cmd,"color ");
    unit=to_hex(font_colour);
    ten=to_hex(background_colour);
    //printf("%c %c\n",unit,ten);//debug
    *(cmd+6)=ten;
    cmd[7]=unit;
    cmd[8]='\0';
    system(cmd);
    //puts(cmd);//debug
    getchar();
    show_default(1,0);
}

void show_colors(int mode)
{
    int input,i;
    const char* colours[]= {"��ɫ","��ɫ","��ɫ","ǳ��ɫ","��ɫ","��ɫ","��ɫ","��ɫ","��ɫ","����ɫ","����ɫ","��ǳ��ɫ","����ɫ","����ɫ","����ɫ","����ɫ"};//used:change_color
    for(i=0; i<16; i++)
        printf("%d. %s   ",i+1,colours[i]);
    printf("\n��ѡ��%s��ɫ:_\b",mode?"����":"����");
    while(!scanf("%d",&input)||input>16||input<1)
    {
        setting_put_tips(1);
        clean_buffer();
        for(i=0; i<16; i++)
            printf("%d. %s   ",i+1,colours[i]);
        printf("\n��ѡ��%s��ɫ:_\b",mode?"����":"����");
    }
    if(mode)
        font_colour=input-1;
    else
        background_colour=input-1;
}

char to_hex(int num)
{
    char ret;
    if (num<10)
        ret='0'+num;
    else
        ret='a'+num-10;
    return ret;
}

void show_about(void)
{
    clean_screen();
    printf("��������Ϸ %s\n\n",STATUS);
    printf("�汾    : v%s_%s\n",FULLVERSION_STRING,STATUS_SHORT);
    printf("��������: %s.%s.%s\n",YEAR,MONTH,DATE);
    printf("\n�����������.\n");
    getch();
    show_default(1,0);
}

void show_points(void)
{
    int total=PLAYER.points+AI.points+NONE.points;

    if(PLAYER.points!=AI.points)
        printf("���ʤ %d ��,�� %d ��   �����ʤ %d ��,�� %d ��   ƽ�� %d ��   %s %d ��   �ܹ� %d ��\n",PLAYER.points,AI.points,AI.points,PLAYER.points,NONE.points,(PLAYER.points>AI.points)?"������ȼ����":"������������",abs(PLAYER.points-AI.points),total);
    else
        printf("���ʤ %d ��,�� %d ��   �����ʤ %d ��,�� %d ��   ���� %d ��   ��Һͼ������ʱ��ƽ   �ܹ� %d ��\n",PLAYER.points,AI.points,AI.points,PLAYER.points,NONE.points,total);

}

void show_help()
{
    int i;
    clean_screen();
    puts("���������");
    putchar('\n');
    puts("��������˳�������");
    putchar('\n');
    puts("���ķ���:");
    puts("    ������������Զ�λ������λ�á��������кţ�Ȼ������ָ���(Ĭ�ϰ�Ƕ���(,))���������кţ���󵥻��س�(Enter)��");
    putchar('\n');
    puts("   1    2    3");
    puts("1  �� |    |   ");
    puts("  --------------");
    puts("2  �� | �� | ��");
    puts("  --------------");
    puts("3     |    | ��");
    puts("       ͼ1");
    putchar('\n');
    puts("    ��ͼ1���������������ԲȦ(��)������Ӧ����:");
    putchar('\n');
    puts("                   2,3");
    putchar('\n');
    printf("�����������ʾ��������");
    getch();
    for(i=1; i<60; i++)
        printf("\b");
    puts("һ��أ���Ҳ��������:");
    putchar('\n');
    puts("                   2-3");
    putchar('\n');
    puts("�����ַ������ǲ��Ƽ���ʹ�á�");
    putchar('\n');
    puts("��Ϸ����");
    puts("    ����ʵ�еľ����������ͬ������һ���ᡢ�ݻ�б�������γ�һ���߼���ʤ����");
    putchar('\n');
    puts("��������˳�������");
    getch();
    show_default(0,0);
    puts("������������꣬�ٵ����س�(Enter)");
}

int change_first(void)
{
    int select;
    clean_screen();
    puts("�������·����ܻ��������̡��Ƿ����?");
    puts("1.�� 2.��");
    printf("��ѡ��:_\b");
    while((!scanf("%d",&select))||(select>3)||(select<1))
    {
        clean_screen();
        clean_buffer();
        puts("�������!�밴��ʾ����!");
        puts("�������·����������̡��Ƿ����?");
        puts("1.�� 2.��");
        printf("��ѡ��:_\b");
    }
    clean_buffer();
    clean_screen();
    if(select==1)
    {
        printf("�������·�: %s \nĬ�����·�: %s\n",((first==PLAYER.num)?"���":"�����"),"���");
        puts("1.��� 2.����� 3.����");
        printf("��������·�����Ϊ:_\b");
        while((!scanf("%d",&select))||(select>4)||(select<1))
        {
            clean_buffer();
            clean_screen();
            puts("�������!�밴��ʾ����!");
            printf("�������·�: %s \n Ĭ�����·�: %s\n",((first==PLAYER.num)?"���":"�����"),"���");
            puts("1.��� 2.����� 3.����");
            printf("��������·�����Ϊ:_\b");
        }
        clean_buffer();
        switch(select)
        {
        case 1:
            if(first!=PLAYER.num)
                ischangefirst=reset=1;
            first=PLAYER.num;
            break;
        case 2:
            if(first!=AI.num)
                ischangefirst=reset=1;
            first=AI.num;
            break;
        case 3:
            show_default(1,0);
            return 0;
        }
        show_default(1,0);
    }
    else
    {
        show_default(1,0);
        return 0;
    }
    return 1;
}
