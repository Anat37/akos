void mapClone(struct steam* ptr)
{
    int i, j, x, y, l;
    int flag;
    ptr->map = malloc(map_size_n * sizeof(char*));
    ptr->objects = malloc(map_size_n * sizeof(struct object*));
    ptr->map_mutex = malloc(map_size_n * sizeof(pthread_mutex_t));
    for (i = 0; i < map_size_n; ++i)
    {
        pthread_mutex_init(&ptr->map_mutex[i]);
        ptr->map[i] = malloc((strlen(map[i]) + 1) * sizeof(char));
        ptr->objects[i] = malloc(map_size_m * sizeof(struct object));
        for (j = 0; j < map_size_m; ++j)
        {
            ptr->objects[i][j].type = -1;
        }
        strcpy(str->map[i], map[i]);
    }
    for (i = 0; i < objectscnt; ++i)
    {
        ptr->objects[objects[i].x][objects[i].y] = mobjects[i];
        ptr->map[objects[i].x][objects[i].y] = '+'
    }
    for (l = 0; l < ptr->max_pl_cnt; ++l)
    {
        x = rand() % map_size_n;
        y = rand() % map_size_m;
        flag = 0;
        for (i = x; i < map_size_n; ++i)
            for(j = y; j < map_size_m; ++j)
                if (ptr->map[i][j] == '*')
                {
                    player_arr[ptr->pl[l]].x = i;
                    player_arr[ptr->pl[l]].y = j;
                    ptr->map[i][j] = '@';
                    flag = 1;
                }
        if (flag == 0)
           for (i = 0; i < map_size_n; ++i)
               for(j = 0; j < map_size_m; ++j)
                   if (ptr->map[i][j] == '*')
                   {
                       player_arr[ptr->pl[l]].x = i;
                       player_arr[ptr->pl[l]].y = j;
                       ptr->map[i][j] = '@';
                       flag = 1;
                   }
        if (flag == 0)
        {
            for (i = l; i < ptr->max_pl_cnt; ++i)
                player_arr[ptr->pl[i]].status = ST_DEAD;
            break;            
        }
        player_arr[ptr->pl[l]].status = ST_ALIVE;
	    player_arr[ptr->pl[l]].hp = initial_health;
	    player_arr[ptr->pl[l]].minecnt = 6;
	    player_arr[ptr->pl[l]].minecd = moratory_duration;
	    player_arr[ptr->pl[l]].guncd = moratory_duration;
    }
}

void mapSend(struct steam* ptr,int fd, int x, int y)
{
    int i, j;
    int left = (y - 10 >= 0) ? 0 : 10 - y;
    int right = (y + 10 <= map_size_m) ? y + 10 : map_size_m;
    char* str = "####################";
    char* buf = malloc(21 * sizeof(char));;
    msg_send(fd, MSG_INFO_STRING, strlen(wait_str) + 1, wait_str);
    for (i = x - 10; i < x + 10; ++i)
    {
        if (i > map_size_n || i < 0){
            strcpy(buf, str);
        } else {
            pthread_mutex_lock(&ptr->mutex[i]);
            for (j = y - 10; j < y + 10; ++j)
                if (j >= 0 && j < map_size_m)
                    buf[j + 10 - y] = ptr->map[i][j];
                else
                    buf[j + 10 - y] = str[j + 10 - y];
            buf[20] = '\0';
            pthread_mutex_unlock(&ptr->mutex[i]);
        }
        msg_send(fd, MSG_MAP_STRING, 21, buf);
    }    
}
void disconnect(struct sthread* ptr){}

void setSend(int fd){
    msg_send(fd, MSG_INFO_STRING, sizeof(set_send) + 1, (void*)&set_send);
    msg_send(fd, MSG_SET_NUM, sizeof(float), (void*)&hit_value);
    msg_send(fd, MSG_SET_NUM, sizeof(int), (void*)&recharge_duration);
    msg_send(fd, MSG_SET_NUM, sizeof(int), (void*)&mining_time);
    msg_send(fd, MSG_SET_NUM, sizeof(float), (void*)&stay_health_drop);
    msg_send(fd, MSG_SET_NUM, sizeof(float), (void*)&movement_health_drop);
    msg_send(fd, MSG_SET_NUM, sizeof(int), (void*)&step_standard_delay);
    msg_send(fd, MSG_SET_NUM, sizeof(int), (void*)&moratory_duration);
}

void wakeSign(int team_id){
    int i, n;
    n = team_arr[team_id].pl_cnt;
    for (i = 0; i < n; ++i)
    {
        pthread_kill(plthread[player_arr[team_arr[team_id].pl[i]].thread_id],SIGUSR1);
    }
}

void wakeCond(int team_id)
{
 int i, n;
    n = team_arr[team_id].pl_cnt;
    for (i = 0; i < n; ++i)
    {
        pthread_cond_signal(&sthreads[player_arr[team_arr[team_id].pl[i]].thread_id].cond,SIGUSR1);
    }   
}

