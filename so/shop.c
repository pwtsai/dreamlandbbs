/*-------------------------------------------------------*/
/* so/shop.c                                             */
/*-------------------------------------------------------*/
/* author : shiun.bbs@ccca.mksh.phc.edu.tw               */
/* modify : cat.bbs@ccca.mksh.phc.edu.tw                 */
/* update : cache.bbs@bbs.ee.ncku.edu.tw                 */
/* create : unknown                                      */
/* update : 09/10/11                                     */
/*-------------------------------------------------------*/
/* syntax :                                              */
/*-------------------------------------------------------*/

#include "bbs.h"

static int cloak_temp(void)
{
    time_t now;
    char c_time[25], c_buf[128];

    now = time(0);
    str_scpy(c_time, ctime(&now), sizeof(c_time));
    ACCT acct;
    int money;

    if (HAS_PERM(PERM_CLOAK))
    {
        pmsg2("你有隱身的權限！");
        return 0;
    }

    if (cutmp->ufo & UFO_CLOAK)
    {
        pmsg2("您已經隱形了！");
        return 0;
    }

    if (acct_load(&acct, cuser.userid) >= 0)
        money = acct.money;
    else
    {
        pmsg2("查無您的帳戶資訊...");
        return 0;
    }


    if (money < 65536)
    {
        pmsg2("您的夢幣不足!!");
        return 0;
    }

    if (vans("確定購買嗎？ [y/N]") != 'y')
        return 0;

    acct.money -= 65536;
    acct_save(&acct);

    cuser.ufo ^= UFO_CLOAK;
    cutmp->ufo ^= UFO_CLOAK;

//  acct_save(&cuser);
    pmsg2("要現身請重新上站");
    sprintf(c_buf, "%s %s 購買 隱身 權限\n", c_time, cuser.userid);
    f_cat(FN_SHOP, c_buf);

    return 0;

}

static int hidefrom_temp(void)
{
    time_t now;
    char c_time[25], c_buf[128];

    now = time(0);
    str_scpy(c_time, ctime(&now), sizeof(c_time));
    ACCT acct;
    int money;

/*
    if (!HAS_PERM(PERM_12))
    {
        vmsg("此功\能暫時關閉");
        return 0;
    }
*/

    if (HAS_PERM(PERM_CLOAK))
    {
        pmsg2("你有隱身的權限！");
        return 0;
    }

    if (cutmp->ufo & UFO_HIDDEN)
    {
        pmsg2("您已經隱藏故鄉了！");
        return 0;
    }

    if (acct_load(&acct, cuser.userid) >= 0)
        money = acct.money;
    else
    {
        pmsg2("查無您的帳戶資訊...");
        return 0;
    }

    if (money < 4194304)
    {
        pmsg2("您的夢幣不足!!");
        return 0;
    }

    if (vans("確定購買嗎？ [y/N]") != 'y')
        return 0;

    acct.money -= 4194304;
    acct_save(&acct);

    cuser.ufo ^= UFO_HIDDEN;
    cutmp->ufo ^= UFO_HIDDEN;

    acct_save(&cuser);
    pmsg2("故鄉已隱藏");
    sprintf(c_buf, "%s %s 購買 隱藏故鄉 權限\n", c_time, cuser.userid);
    f_cat(FN_SHOP, c_buf);

    return 0;

}

static int sysop(void)
{
    pmsg2("是誰說你可以當站長的！");
    return 0;
}

int shop_main(void)
{
    DL_HOLD;
    char buf[5];
    int money;

    ACCT acct;
    if (acct_load(&acct, cuser.userid) >= 0)
    {
        money = acct.money;
    }
    else
    {
        pmsg2("查無您的帳戶資訊...");
        return DL_RELEASE(0);
    }

    clear();

    move(0, 0);
    prints("\x1b[1;33;42m");
    vs_mid(BOARDNAME "    商店街");
    prints("\n");
    prints("                     您的現金如下：\n\n                     夢幣：%d 元", money);
    move(10, 0);

    outs("          歡迎進入商店街，請問您需要什麼服務：\n"
        "  \n"
        "            (1) 購買暫時隱身術   (需要夢幣   65536 元/次)\n\n"
        "            (2) 購買永久隱身術   (需要夢幣  1048576 元/次)\n\n"
        "            (3) 購買隱藏故鄉     (需要夢幣  4194304 元/次)\n\n"
        "            (4) 購買站長權限\x1b[1;31m HOT\x1b[m (需要夢幣 10000000 元)\n");

    if (!vget(B_LINES_REF, 0, "請選擇您要的服務： [Q] 離開 ", buf, 2, DOECHO))
        return DL_RELEASE(0);

    if (*buf == '1')
        cloak_temp();
    else if (*buf == '2')
        pmsg2("此功\能尚未開放");
    else if (*buf == '3')
        pmsg2("Sorry, we've closed this function...");
        //hidefrom_temp();
    else if (*buf == '4')
        sysop();
    else
        pmsg2("謝謝光臨");
    return DL_RELEASE(0);
}
