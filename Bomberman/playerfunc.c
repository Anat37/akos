#include "header.h"

/*

extern void err_report(const char* str, short flag);
extern void free_mem();*/

int player_handler(int sig){
    printf("Give me a sign!");
    return 0;
}



void statSend(struct sthread* ptr)
{
    msg_send(ptr->desc, MSG_INFO_STRING, strlen(st_send) + 1, (void*)st_send);
    msg_send(ptr->desc, MSG_STAT_NUM, sizeof(int), (void*)&player_arr[ptr->pl].x);
    msg_send(ptr->desc, MSG_STAT_NUM, sizeof(int), (void*)&player_arr[ptr->pl].y);
    msg_send(ptr->desc, MSG_STAT_NUM, sizeof(float), (void*)&player_arr[ptr->pl].hp);
    printf("Send hp %f\n", player_arr[ptr->pl].hp);
    msg_send(ptr->desc, MSG_STAT_NUM, sizeof(int), (void*)&player_arr[ptr->pl].minecnt);
    msg_send(ptr->desc, MSG_STAT_NUM, sizeof(int), (void*)&player_arr[ptr->pl].minecd);
    msg_send(ptr->desc, MSG_STAT_NUM, sizeof(int), (void*)&player_arr[ptr->pl].guncd);
}
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
 
void upd_status(struct sthread* ptr){
    int pl = ptr->pl;
    long k = time_from(player_arr[pl].lasthp);
    player_arr[pl].minecd -= k;
    player_arr[pl].guncd -= k;
    player_arr[pl].hp -= stay_health_drop * (k / step_standard_delay);
    player_arr[pl].lasthp = time_inc(player_arr[pl].lasthp, k / step_standard_delay);
    if (team_arr[ptr->team_id].objects[player_arr[pl].x][player_arr[pl].y].type == OBJ_MINE && 
    team_arr[ptr->team_id].objects[player_arr[pl].x][player_arr[pl].y].owner != pl)
    {
        team_arr[ptr->team_id].objects[player_arr[pl].x][player_arr[pl].y].type = -1;
        player_arr[pl].hp -= hit_value;
    }
    if (player_arr[pl].hp < 0)
    {
        player_arr[pl].status = ST_DEAD;
        team_arr[ptr->team_id].map[player_arr[pl].x][player_arr[pl].y] = ' ';
        return;
    }
    statSend(ptr);
}

void damage(int team, int x, int y, float k){
    int i, pl;
    for (i = 0; i < team_arr[team].pl_cnt; ++i)
    {
        if (player_arr[pl].status == ST_DEAD)
          continue;
        pl = team_arr[team].pl[i];
        if (player_arr[pl].x == x &&
            player_arr[pl].y == y)
            {
                printf("Here comes the bfs!!!!!\n");
                player_arr[pl].hp -= k * hit_value;
                printf("Here %d\n",player_arr[pl].thread_id);
                pthread_kill(plthread[player_arr[pl].thread_id], SIGUSR1);
                break;
            }
    }
}
void bfs(struct sthread* ptr)
{
    char** map = team_arr[ptr->team_id].map;
    int x = player_arr[ptr->pl].x;
    int y = player_arr[ptr->pl].y;
    int i, j;
    float l;
    printf("Here comes the bfs!!!!!\n");
    pthread_mutex_lock(&team_arr[ptr->team_id].mutex);
    printf("Here comes the bfs!!!!!\n");
    if (x + 1 < map_size_n && (map[x+1][y] == ' ' || map[x+1][y] == '+'))
        map[x+1][y] = 'F';
    if (x + 1 < map_size_n && (map[x+1][y] == '@'))
    {
        map[x+1][y] = 'B';
        damage(ptr->team_id, x + 1, y, 1);
    }
    
    if (y + 1 < map_size_m && (map[x][y + 1] == ' ' || map[x][y + 1] == '+'))
        map[x][y + 1] = 'F';
    if (y + 1 < map_size_m && (map[x][y + 1] == '@'))
    {
        map[x][y + 1] = 'B';
        damage(ptr->team_id, x, y + 1, 1);
    }
    
    if (x - 1 > 0 && (map[x - 1][y] == ' ' || map[x - 1][y] == '+'))
        map[x - 1][y] = 'F';
    if (x - 1 > 0 && (map[x - 1][y] == '@'))
    {
        map[x - 1][y] = 'B';
        damage(ptr->team_id, x - 1, y, 1);
    }   
    
    if (y - 1 > 0 && (map[x][y - 1] == ' ' || map[x][y - 1] == '+'))
        map[x][y - 1] = 'F';
    if (y - 1 > 0 && (map[x][y - 1] == '@'))
    {
        map[x][y - 1] = 'B';
        damage(ptr->team_id, x, y - 1, 1);
    }
    
    for (l = 0.9; l > 0; l-=0.1)
        for (i = 0; i < map_size_n; ++i)
            for(j = 0; j < map_size_m; ++j)
            {
                if (x == i && y == j)
                    continue;
                if ((i + 1 < map_size_n && map[i + 1][j] == 'F') ||
                    (j + 1 < map_size_m && map[i][j + 1] == 'F') ||
                    (i - 1 > 0 && map[i - 1][j] == 'F') ||
                    (j - 1 > 0 && map[i][j - 1] == 'F') )
                    {
                        if (map[i][j] == ' ' || map[i][j] == '+')
                            map[i][j] = 'F';
                        if (map[i][j] == '@')
                            damage(ptr->team_id, i, j, l);
                    }
            }
            
    mapSend(&team_arr[ptr->team_id],ptr->desc,x,y);
    
    for (i = 0; i < map_size_n; ++i)
            for(j = 0; j < map_size_m; ++j)
            {
                if (map[i][j] == 'F')
                    if (team_arr[ptr->team_id].objects[i][j].type == OBJ_MAP)
                        map[i][j] = '+';
                    else
                        map[i][j] = ' ';
                
                if (map[i][j] == 'B')
                  map[i][j] == '@';
            } 
    pthread_mutex_unlock(&team_arr[ptr->team_id].mutex);            
}

void move(struct sthread* ptr, int k){
    int oldx = player_arr[ptr->pl].x;
    int oldy = player_arr[ptr->pl].y;
    int newx = oldx;
    int newy = oldy;
    int pl = ptr->pl;
    if (time_from(player_arr[ptr->pl].lastmove) < 1 || player_arr[pl].minecd > recharge_duration)
    {
        msg_send(ptr->desc, MSG_INFO_STRING, strlen(bad_mov) + 1, (void*) bad_mov);
        return;
    }
    if (k == UP)
        --newx;
    if (k == DOWN)
        ++newx;
    if (k == LEFT)
        --newy;
    if (k == RIGHT)
        ++newy;
    pthread_mutex_lock(&team_arr[ptr->team_id].map_mutex[oldx]);
    if (oldx != newx)
        pthread_mutex_lock(&team_arr[ptr->team_id].map_mutex[newx]);
        if (team_arr[ptr->team_id].map[newx][newy] == ' ' || team_arr[ptr->team_id].map[newx][newy] == '+')
        {
            team_arr[ptr->team_id].map[newx][newy] = '@';
            if (team_arr[ptr->team_id].objects[oldx][oldy].type == OBJ_MAP)
              team_arr[ptr->team_id].map[oldx][oldy] = '+';
            else 
              team_arr[ptr->team_id].map[oldx][oldy] = ' ';
            player_arr[ptr->pl].x = newx;
            player_arr[ptr->pl].y = newy;
            player_arr[ptr->pl].hp -= movement_health_drop - stay_health_drop;
            k = clock_gettime(CLOCK_MONOTONIC, &player_arr[ptr->pl].lastmove); 
            msg_send(ptr->desc, MSG_INFO_STRING, strlen(succ_mov) + 1, (void*)succ_mov);
            upd_status(ptr);
        } else 
        {
            msg_send(ptr->desc, MSG_INFO_STRING, strlen(bad_mov) + 1, (void*) bad_mov);
        }
    
    pthread_mutex_unlock(&team_arr[ptr->team_id].map_mutex[oldx]);
    if (oldx != newx)
        pthread_mutex_unlock(&team_arr[ptr->team_id].map_mutex[newx]);
    wakeSign(ptr->team_id);
    return;    
}

void use_object(struct sthread* ptr)
{
    int pl = ptr->pl;
    int k;
    if (time_from(player_arr[ptr->pl].lastmove) < 1|| player_arr[pl].minecd > recharge_duration)
    {
        msg_send(ptr->desc, MSG_INFO_STRING, strlen(bad_mov) + 1, (void*) bad_mov);
        return;
    }
    if (team_arr[ptr->team_id].objects[player_arr[pl].x][player_arr[pl].y].type == OBJ_MAP)
    {
        team_arr[ptr->team_id].objects[player_arr[pl].x][player_arr[pl].y].type = -1;
        player_arr[pl].hp += team_arr[ptr->team_id].objects[player_arr[pl].x][player_arr[pl].y].arg;
        k = clock_gettime(CLOCK_MONOTONIC, &player_arr[ptr->pl].lastmove); 
        msg_send(ptr->desc, MSG_INFO_STRING, strlen(succ_mov) + 1, (void*)succ_mov);
        upd_status(ptr);
    } 
    else
    {
       msg_send(ptr->desc, MSG_INFO_STRING, strlen(bad_mov) + 1, (void*) bad_mov);
       return; 
    }
}
void put_min(struct sthread* ptr)
{
    int pl = ptr->pl;
    int k;
    if (time_from(player_arr[ptr->pl].lastmove) < 1 || player_arr[pl].minecd > recharge_duration)
    {
        msg_send(ptr->desc, MSG_INFO_STRING, strlen(bad_mov) + 1, (void*) bad_mov);
        return;
    }
    if (time_from(team_arr[ptr->team_id].start) < moratory_duration/step_standard_delay)
    {
        msg_send(ptr->desc, MSG_INFO_STRING, strlen(bad_mov) + 1, (void*) bad_mov);
        return;
    }
    if (player_arr[pl].minecd > 0)
    {
        msg_send(ptr->desc, MSG_INFO_STRING, strlen(bad_mov) + 1, (void*) bad_mov);
        return;
    }
    if (player_arr[pl].minecnt < 0)
    {
        msg_send(ptr->desc, MSG_INFO_STRING, strlen(bad_mov) + 1, (void*) bad_mov);
        return;
    }
    --player_arr[pl].minecnt;
    player_arr[pl].minecd = mining_time + recharge_duration;
    team_arr[ptr->team_id].objects[player_arr[pl].x][player_arr[pl].y].type = OBJ_MINE;
    team_arr[ptr->team_id].objects[player_arr[pl].x][player_arr[pl].y].owner = pl;
    k = clock_gettime(CLOCK_MONOTONIC, &player_arr[ptr->pl].lastmove); 
    msg_send(ptr->desc, MSG_INFO_STRING, strlen(succ_mov) + 1, (void*)succ_mov);
    upd_status(ptr);
}
void gun_use(struct sthread* ptr)
{
    int pl = ptr->pl;
    int k;
    if (time_from(player_arr[ptr->pl].lastmove) < 1 || player_arr[pl].minecd > recharge_duration)
    {
        msg_send(ptr->desc, MSG_INFO_STRING, strlen(bad_mov) + 1, (void*) bad_mov);
        return;
    }
    if (time_from(team_arr[ptr->team_id].start) < moratory_duration/step_standard_delay)
    {
        msg_send(ptr->desc, MSG_INFO_STRING, strlen(bad_mov) + 1, (void*) bad_mov);
        return;
    }
    if (player_arr[pl].guncd > 0)
    {
        msg_send(ptr->desc, MSG_INFO_STRING, strlen(bad_mov) + 1, (void*) bad_mov);
        return;
    }
    bfs(ptr); 
    player_arr[pl].guncd = recharge_duration;
    k = clock_gettime(CLOCK_MONOTONIC, &player_arr[ptr->pl].lastmove); 
    msg_send(ptr->desc, MSG_INFO_STRING, strlen(succ_mov) + 1, (void*)succ_mov);
    upd_status(ptr);    
}
/*
 *const char* up_m = "Move up";
 *const char* dw_m = "Move down";
 *const char* lf_m = "Move left";
 *const char* rt_m = "Move right";
 *const char* put_mine = "Put mine";
 *const char* us_ob = "Use object";
 *const char* ch_st = "Change status";
 *const char* fire = "Use gun";
 */

int pl_move(struct sthread* ptr, char* str){
    if (!strcmp(str, up_m))
    {
        move(ptr, UP);
        return 0;
    }
    if (!strcmp(str, dw_m))
    {
        move(ptr, DOWN);
        return 0;
    }
    if (!strcmp(str, lf_m))
    {
        move(ptr, LEFT);
        return 0;
    }
    if (!strcmp(str, rt_m))
    {
        move(ptr, RIGHT);
        return 0;
    }
    if (!strcmp(str, fire))
    {
        gun_use(ptr);
        wakeSign(ptr->team_id);
        return 0;
    }
    if (!strcmp(str, put_mine))
    {
        put_min(ptr);
        return 0;
    }
    if (!strcmp(str, us_ob))
    {
        use_object(ptr);
        return 0;
    }
    if (!strcmp(str, ch_st))
    {
        upd_status(ptr);
        return 0;
    }
    if (!strcmp(str, disc))
    {
       return -1;
    }
    return 0;
}


void player_gameplay(struct sthread* ptr){
    int poll_ret;
    int flag = 1;
    struct pollfd polls;
    size_t size = 0;
    int type = 0;
    int msg_ret = 0;
    void* buf = NULL;
    
    printf("In play game\n");
    
    polls.fd = ptr->desc;
    polls.events = POLLIN | POLLERR | POLLHUP;
    polls.revents = 0;
    while (flag){
        printf("POLLIN\n");
        poll_ret = poll(&polls, 1, -1);
        pthread_mutex_lock(&team_arr[ptr->team_id].mutex);
            if (team_arr[ptr->team_id].status == ST_PLAYING)
            {
                flag = 0;
                printf("Proceed\n");
                pthread_mutex_unlock(&team_arr[ptr->team_id].mutex);
                continue;
            }
        pthread_mutex_unlock(&team_arr[ptr->team_id].mutex);
        if (poll_ret == -1)
        {
            err_report("poll failed\n", 1);
        }
        if (polls.revents & POLLERR) {
            err_report("poll returned POLLERR\n", 0);
            return;
        }
        if (polls.revents & POLLHUP) {
            err_report("poll returned POLLHUP\n", 0);
            return;
        }
        if (polls.revents & POLLIN){
            msg_ret = msg_rec(ptr->desc, &type, &size, &buf);
            if (type == MSG_INFO_STRING && !strcmp((char*)buf,disc)){
                disconnect(ptr);
                return;
            }
        }
        polls.revents = 0;       
    }
    msg_send(ptr->desc, MSG_INFO_STRING, strlen(start) + 1, start);
    setSend(ptr->desc);
    mapSend(&team_arr[ptr->team_id], ptr->desc, player_arr[ptr->pl].x, player_arr[ptr->pl].y);
    statSend(ptr);
    pthread_mutex_lock(&team_arr[ptr->team_id].mutex);
    ++team_arr[ptr->team_id].ready_cnt;
    if (team_arr[ptr->team_id].ready_cnt == team_arr[ptr->team_id].pl_cnt)
    {
        pthread_mutex_unlock(&team_arr[ptr->team_id].mutex);
        pthread_kill(plthread[team_arr[ptr->team_id].thread_id], SIGUSR2);       
    } 
    else
    {
        pthread_mutex_unlock(&team_arr[ptr->team_id].mutex);
        pthread_mutex_lock(&ptr->mutex);
        pthread_cond_wait(&ptr->cond, &ptr->mutex);
        pthread_mutex_unlock(&ptr->mutex);
    }
    msg_send(ptr->desc, MSG_INFO_STRING, strlen(start) + 1, start);
    msg_ret = clock_gettime(CLOCK_MONOTONIC, &player_arr[ptr->pl].lastmove);
    msg_ret = clock_gettime(CLOCK_MONOTONIC, &player_arr[ptr->pl].lasthp);
    flag = 1;
    while (flag){
        poll_ret = poll(&polls, 1, -1);
        if (poll_ret == -1)
        {
            printf("Caught sig\n");
            err_report("poll failed\n", 1);
            
        }
        if (polls.revents & POLLERR) {
            err_report("poll returned POLLERR\n", 0);
            return;
        }
        if (polls.revents & POLLHUP) {
            err_report("poll returned POLLHUP\n", 0);
            return;
        }
        if (polls.revents & POLLIN){
            msg_ret = msg_rec(ptr->desc, &type, &size, &buf);
            if (type == MSG_INFO_STRING){
                if (pl_move(ptr, buf) != 0)
                    flag = 0;
            }
        }
        printf("End move\n");
        upd_status(ptr);
        mapSend(&team_arr[ptr->team_id], ptr->desc, player_arr[ptr->pl].x, player_arr[ptr->pl].y);
        polls.revents = 0;  
        if (player_arr[ptr->pl].status == ST_DEAD)
        {
            msg_send(ptr->desc, MSG_INFO_STRING, strlen(disc) + 1, (void*)disc);
            flag = 0;
        }    
    }
    free(buf);
    disconnect(ptr);
}
