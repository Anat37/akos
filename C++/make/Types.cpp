#include "Types.h"

Node::Node(const Node& sample)
{
    left_header = sample.left_header;
    right_header = sample.right_header;
    data = sample.data;
}

void Node::append_left_header(string sample)
{
    left_header = sample;
}

void Node::append_right_header(vector<string> sample)
{
    right_header = sample;
}

void Node::append(string sample)
{
    data.push_back(sample);
}

void Node::print()
{
    cout<<"left header = "<<left_header<<"; right_header = ";
    for(int i = 0; i<right_header.size(); i++)
        cout<<right_header[i]<<' ';
    cout<<endl;

    for(int i = 0; i < data.size() ;i++)
    {
        cout<<data[i]<<endl;
    }
}

string& Node::operator [](const int pos)
{
    return data[pos];
}

Node& Node::operator = (const Node& sample)
{
    left_header = sample.left_header;
    right_header = sample.right_header;
    data = sample.data;
    return *this;
}

//----------------------------------------------------------------------------------------------------------------------------------

void Module::print()
{
    for(int i = 0; i<data.size(); i++)
    {
        cout<<"Module №"<<i+1<<':'<<endl;
        data[i].print();
        cout<<endl;
    }
}

void Module::append(Node sample)
{
    data.push_back(sample);
}

Node& Module::operator [](const int pos)
{
    return data[pos];
}

//----------------------------------------------------------------------------------------------------------------------------------


vector<string> Parser::split(string sample)
{
    vector<string> ans;
    int i, last_pos = 0;
    
    for(i = 0; sample[i] ;i++)
    {
        if (sample[i] == ' ')
        {
            ans.push_back(sample.substr(last_pos, i-last_pos));
            last_pos = i+1;
        }
    }
    if (i!= last_pos)
        ans.push_back(sample.substr(last_pos, i-last_pos));

    return ans;
}

string Parser::strip(string sample)
{
    string tmp;
    int i = 0, 
    space = 0;

    while((sample[i] == ' ')||(sample[i] == '\t'))
        ++i;
    
    while(sample[i])
    {
        if (sample[i] == ' ')
        {
            space = 1;
        }else
        {
            if(space)
            {
                tmp.push_back(' ');
                space = 0;
            }
            tmp.push_back(sample[i]);
        }
        ++i;
    }
    return tmp;
}

string Parser::itoa(int number)
{
    char buff[21]; // enought for int
    sprintf(buff, "%i", number);
    return string(buff);
}

int Parser::get_index(vector<int> sample, int key)
{
    for(int i = 0; i<sample.size(); i++)
        if (sample[i] == key)
            return i;
    return -1;
}

int Parser::get_mod_index(string& vertex)
{
    for(int i=0; i<mod.data.size(); i++)
    {
        if (!mod[i].left_header.compare(vertex))
            return i;
    }
    throw string("Graph error, there are no vertext: ")+vertex;
    return -1;
}

void Parser::get_c()
{
    ++column;
    c = fgetc(f);
}

int Parser::is_file(string& filename)
{
    struct stat buf;
    if ((stat(filename.c_str(), &buf)>=0) && (S_ISREG(buf.st_mode)))
        return 1;
    return 0;
}

void Parser::assemble(int current)
{
    int index;
    for(int i = 0; i<mod[current].right_header.size(); i++)
    {
        if (get_index(waiting, current) != -1)
            throw string("Queue error");

        if(!is_file(mod[current].right_header[i]))
        {
            index = get_mod_index(mod[current].right_header[i]);
            if (get_index(collected, index) == -1)
            {
                waiting.push_back(current);
                assemble(index);
                waiting.pop_back();
            }
        }
    }

    collected.push_back(current);
    
    for(int i = 0; i < mod[current].data.size(); i++)
    {
        cout<<mod[current][i].c_str()<<endl;
        if (system(mod[current][i].c_str())!=0)
           throw string("Execution error on module ")+itoa(current)+string(" on line ")+itoa(i+1);
    }
}

Parser::Parser(int argc, char** argv)
{
    line = 1;
    pos = 0;
    debug = 0;
    prev_state = LEX_NONE;
    state = LEX_NONE;
    var_left_bracket = 0;
    var_header = 0;

    f = fopen("tmp_makefile","r");

    switch(getopt(argc,argv,"df:"))
    {
        case 'f':
            if (f!= NULL)
                fclose(f);
            f = fopen(optarg, "r");
            break;
        
        case 'd':
            cout<<"debug is on!"<<endl;
            debug = 1;
            break;
    }

    if (f==NULL)
    {
        cerr<<string("Thire is no such file!")<<endl;
        exit(1);
    }
    
    if (optind<argc)
    {
        node_to_run = string(argv[optind]);
    }
}

void Parser::load()
{
    do
    {
        switch(state)
        {
            case LEX_NONE:
                get_c();
                state = LEX_H;
                if (debug)
                    cout<<"\t<DEBUG START>"<<endl;
                break;

            case LEX_H:
                switch(c)
                {
                    case -1:
                        state = LEX_END;
                        if (debug)
                        {
                            cout<<"\t<DEBUG END>"<<endl;
                        }
                        break;

                    case '\n':
                        column = 0;
                        ++line;
                        var_header = 0;
                        get_c();
                        if (debug)
                        {
                            cout<<"<EMPTY>"<<endl;
                        }
                        break;

                    case '#':
                        state = LEX_COMMENT;
                        get_c();
                        if (debug)
                        {
                            cout<<"<COMMENT>#";
                        }
                        break;

                    case '\t':
                        state = LEX_LINE;
                        get_c();
                        if (debug)
                        {
                            cout<<"<LINE>";
                        }
                        break;

                    default:
                        state = LEX_LEFT;
                        if (debug)
                        {
                            cout<<"<LEFT>";
                        }
                        break;
                }
                break;

            case LEX_LEFT:
                switch(c)
                {
                    case ':':
                        mod.append(Node());
                        ++pos;
                        var_header = 1;
                        mod[pos-1].append_left_header(strip(buffer));
                        buffer.clear();
                        state = LEX_RIGHT_HEADER;
                        if (debug)
                        {
                            cout<<"</LEFT><COLON>"<<(char)c<<"</COLON><RIGHT PART OF HEADER>";
                        }
                        break;

                    case '#':
                        buffer.clear();
                        throw string("Line type error on: (")+itoa(line)+string(",")+itoa(column)+string(")");
                        break;

                    case '=':
                        var_key = strip(buffer);
                        buffer.clear();
                        state = LEX_RIGHT_VAR;
                        if (debug)
                        {
                            cout<<"</LEFT><EQUALITY>"<<(char)c<<"</EQUALITY><RIGHT PART OF VARIABLE>";
                        }
                        break;

                    case '$':
                        prev_state = LEX_LEFT;
                        state = LEX_VAR;
                        if (debug)
                        {
                            cout<<"<VARIABLE>"<<(char)c;
                        }
                        break;

                    case '\n':
                    case -1:
                        buffer.clear();
                        throw string("Line type error on: (")+itoa(line)+string(",")+itoa(column)+string(")");
                        break;

                    default:
                        if (isprint(c))
                            buffer.push_back(c);                
                        if (debug)
                        {
                            cout<<(char)c;
                        }
                        break;
                }
                get_c();
                break;

            case LEX_RIGHT_HEADER:
                switch(c)
                {
                    case ':':
                        buffer.clear();
                        throw string("Header error on: (")+itoa(line)+string(",")+itoa(column)+string(")");
                        break;

                    case '\n':
                    case -1:
                        column = 0;
                        ++line;
                        mod[pos-1].append_right_header(split(strip(buffer)));
                        buffer.clear();
                        state = LEX_H;
                        if (debug)
                        {
                            cout<<"</RIGHT PART OF HEADER>"<<endl;
                        }
                        break;

                    case '#':
                        mod[pos-1].append_right_header(split(strip(buffer)));
                        buffer.clear();
                        state = LEX_COMMENT;
                        if (debug)
                        {
                            cout<<"<COMMENT>#";
                        }
                        break;

                    case '=':
                        buffer.clear();
                        throw string("Line type error on: (")+itoa(line)+string(",")+itoa(column)+string(")");
                        break;

                    case '$':
                        prev_state = LEX_RIGHT_HEADER;
                        state = LEX_VAR;
                        if (debug)
                        {
                            cout<<"<VARIABLE>"<<(char)c;
                        }
                        break;

                    default:
                        if (isprint(c))
                            buffer.push_back(c);                
                        if (debug)
                        {
                            cout<<(char)c;
                        }
                        break;
                }
                get_c();
                break;

            case LEX_RIGHT_VAR:
                switch(c)
                {
                    case '=':
                        buffer.clear();
                        throw string("Variable error on: (")+itoa(line)+string(",")+itoa(column)+string(")");
                        break;

                    case ':':
                        buffer.clear();
                        throw string("Line type error on: (")+itoa(line)+string(",")+itoa(column)+string(")");
                        break;
                        
                    case '\n':
                    case -1:
                        column = 0;
                        ++line;
                        dict[var_key] = strip(buffer);
                        buffer.clear();
                        state = LEX_H;
                        if (debug)
                        {
                            cout<<"</VARIABLE>"<<endl;
                        }
                        break;

                    case '#':
                        dict[var_key] = strip(buffer);
                        buffer.clear();
                        state = LEX_COMMENT;
                        if (debug)
                        {
                            cout<<"<COMMENT>#";
                        }
                        break;

                    default:
                        if (isprint(c))
                            buffer.push_back(c);                
                        if (debug)
                        {
                            cout<<(char)c;
                        }
                        break;
                }
                get_c();
                break;

            case LEX_LINE:
                if (!var_header)
                {
                    buffer.clear();
                    throw string("Header error on: (")+itoa(line)+string(",")+itoa(column)+string(")");
                }
                switch(c)
                {
                    case '\n':
                    case -1:
                        column = 0;
                        ++line;
                        mod[pos-1].append(strip(buffer));
                        buffer.clear();
                        state = LEX_H;
                        if (debug)
                        {
                            cout<<"</LINE>"<<endl;
                        }
                        break;
                    
                    case '$':
                        prev_state = LEX_LINE;
                        state = LEX_VAR;
                        if (debug)
                        {
                            cout<<"<VARIABLE>";
                        }
                        break;

                    case '#':
                        mod[pos-1].append(strip(buffer));
                        buffer.clear();
                        state = LEX_COMMENT;
                        if (debug)
                        {
                            cout<<"<COMMENT#";
                        }
                        break;

                    default:
                        if (isprint(c))
                            buffer.push_back(c);
                        if (debug)
                        {
                            cout<<(char)c;
                        }
                        break;
                }
                get_c();
                break;
            
            case LEX_VAR:
                if ((c=='(')&&(!var_left_bracket))
                {
                    var_left_bracket = 1;
                    if (debug)
                    {
                        cout<<(char)c;
                    }
                    get_c();
                    break;
                }
                
                switch(c)
                {
                    case '(':
                        buffer.clear();
                        var_buffer.clear();
                        throw string("Brackets error on: (")+itoa(line)+string(",")+itoa(column)+string(")");
                        break;

                    case ')':
                    {
                        string ans;
                        try
                        {
                            ans = dict[ var_buffer ];
                        }
                        catch(string e)
                        {
                            buffer.clear();
                            var_buffer.clear();
                            throw e+string("on: (")+itoa(line)+string(",")+itoa(column)+string(")");
                        }
                        
                        int len = ans.length();
                        for(int i = 0; i<len; i++)
                            buffer.push_back(ans[i]);
                        
                        var_buffer.clear();
                        var_left_bracket = 0;
                        state = prev_state;
                        if (debug)
                        {
                            cout<<")</VARIABLE>";
                        }
                        break;
                    }

                    case '\n':
                    case -1:
                        buffer.clear();
                        var_buffer.clear();
                        throw string("Brackets error on: (")+itoa(line)+string(",")+itoa(column)+string(")");

                    case '#':
                        buffer.clear();
                        var_buffer.clear();
                        throw string("Brackets error on line: ")+itoa(line);

                    default:
                        if (isprint(c))
                            var_buffer.push_back(c);
                        if (debug)
                        {
                            cout<<(char)c;
                        }
                        break;
                }
                get_c();
                break;

            case LEX_COMMENT:
                switch(c)
                {
                    case '\n':
                    case -1:
                        column = 0;
                        ++line;
                        state = LEX_H;
                        if (debug)
                        {
                            cout<<"</COMMENT>";
                        }
                        break;
                }
                if (debug)
                {
                    cout<<(char)c;
                }
                get_c();
                break;

            case LEX_END:
                exit(1); // в теории сюда дойти нельзя
        }
    }while(state != LEX_END);
}

void Parser::print()
{
    cout<<"--Start--"<<endl;
    cout<<"--Modules--"<<endl;
    mod.print();

    cout<<"--Dictionary--"<<endl;
    for(map<string, string>::iterator i = dict.begin(); i != dict.end(); i++)
        cout<<i->first<<" : "<<i->second<<endl;
    cout<<"--End--"<<endl;
}

void Parser::collect()
{
    if (!node_to_run.empty())
        assemble(get_mod_index(node_to_run));
    else
        assemble(0);
}

Parser::~Parser()
{
    fclose(f);
}
