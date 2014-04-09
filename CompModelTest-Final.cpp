#include<bits/stdc++.h>
using namespace std;

map <string,vector<string> > m_nt,p,f; // m_nt first set of NT
map <string,string> m_term;//first set of terminals
vector<string> nt,t;
vector<string> prod;
char *nts,*ts;
map<string,vector<string> > :: iterator m_t,p_t;
vector<string>::iterator v;
stack <string> actual;
string **table;

int selectRow(string str)
{
	for(int i=0;i<nt.size();i++)
	{
		if(nt[i]==str)
			return i;
	}
	return -1;
}
int selectColumn(string str)
{
	for(int i=0;i<t.size();i++)
	{
		if(t[i]==str)
			return i;
	}
	return -1;
}
char* getTokens(string str,const char *pattern)
{
	char *nterm;
	char *token;
	nterm=new char[str.length()+1];
	strcpy(nterm,str.c_str());
	token=new char[str.length()+1];
	token=strtok(nterm,pattern);
}
bool isThere(vector<string> q,string z)
{
	for(int i=0;i<q.size();i++)
	{
		if(q[i]==z)
			return true;
	}
	return false;
}

void findfollow()
{
   vector<string> trail,u;
   vector<string>::iterator it;
   int precount=-1,count=0;
   char *token,*tmp;
   bool changing=true;
   p_t=p.begin();
   f[p_t->first].push_back("$");
   while(changing)
   {
      for(p_t=p.begin();p_t!=p.end();p_t++)
      {
          trail=f[p_t->first];
          /*for(int i=0;i<trail.size();i++)
           cout<<"trail"<<trail[i]<<" ";
           cout<<endl;*/
          u=(p_t->second);
          for(int j=0;j<u.size();j++)
          {
           token=getTokens(u[j]," "); // gets first character of right hand side of production
           tmp=token;
           vector<string> tk;
           while(tmp!=0)
				{
					tk.push_back(tmp);
					tmp=strtok(NULL," ");
				}
			//trail.clear();	
          for(int i=tk.size()-1;i>=0;i--)
          {
          	      //cout<<tk[i]<<endl;
          	    //getch();
                if(m_term[tk[i]]=="")
                {
                   for(int k=0;k<trail.size();k++)
                   {
				     //cout<<"trail:"<<trail[k]<<endl;
				     //getch();
					 if (!isThere(f[tk[i]],trail[k]))
				    { 
					 f[tk[i]].push_back(trail[k]);
					 count++;
				    }
				}
				   
                
                   ///cout<<"Tokens:"<<tk[i]<<endl;
                   bool epsilon=false;
                   for(int k=0;k<m_nt[tk[i]].size();k++)	// pushes first set of
										if(m_nt[tk[i]][k]=="^")
											epsilon=true;
					if(epsilon)						
                    { 
					 //trail.insert(trail.end(),m_nt[tk[i]].begin(),m_nt[tk[i]].end());
					 for(it=m_nt[tk[i]].begin();it!=m_nt[tk[i]].end();it++)
                    {
				     if (!isThere(trail,*it)&&*it!="^")
				     { 
					  //cout<<"it"<<*it;
					  trail.push_back(*it);
					  
				     }
			         }
				   }
                    else
                    { 
					 trail.clear();
					 trail.insert(trail.begin(),m_nt[tk[i]].begin(),m_nt[tk[i]].end());
				     }
                }
                else
                  {  
					trail.clear();
					trail.push_back(tk[i]);
			      }
          }
      }
    }
    if(count==precount)
     changing=false;
    else
      precount=count;
  }
}

void parse(vector<string> str)
{
	stack<string> dummy;
	str.push_back("$");
	int index=0;
	p_t=p.begin();
	actual.push("$");
	actual.push(p_t->first);
	while(!actual.empty())
	{
		cout<<actual.top()<<" "<<str[index]<<endl;
		if(actual.top()=="^")
		{
			actual.pop();
			continue;
		}
		if(actual.top()==str[index])
		{
			index++;
			actual.pop();
		}
		else
		{
			string tempo="";
			if(isThere(nt,actual.top()))
				tempo=table[selectRow(actual.top())][selectColumn(str[index])];
			char *w=getTokens(tempo," ");
			if(tempo=="" || actual.top()=="$")
				break;
			while(w!=0)
			{
				dummy.push(w);
				w=strtok(NULL," \n");
			}
			actual.pop();
			while(!dummy.empty())
			{
				actual.push(dummy.top());
				dummy.pop();
			}
		}
	}
	if(index==str.size())
	{
		printf("Parsed Successfully...\n");
	}
	else
	{
		printf("Parse Unsuccessful...\n");
	}
}

void parseTableConstruct()
{
	table=new string*[nt.size()];
	for(int i=0;i<nt.size();i++)
	{
		table[i]=new string[t.size()];
	}
	int rowNo=0,colNo=0;
	//cout<<"\n";
	for(p_t=p.begin();p_t!=p.end();p_t++)
	{
		for(v=p_t->second.begin();v!=p_t->second.end();v++)
		{
			//cout<<*v<<endl;
			if(*v=="^")
			{
				continue;
			}
			else
			{
				//cout<<"noep:"<<p_t->first<<" :"<<*v<<":"<<endl;
				rowNo=selectRow(p_t->first);
				for(int j=0;j<m_nt[p_t->first].size();j++)
				{
					colNo=selectColumn(m_nt[p_t->first][j]);
					if(table[rowNo][colNo]=="")
						table[rowNo][colNo]+=*v;
					else
					{
						fprintf(stderr,"FIRST-FIRST Conflict! Not a LL(1) grammar");
						exit(0);
					}
				}
			}
		}
	}
	for(p_t=p.begin();p_t!=p.end();p_t++)
	{
		for(v=p_t->second.begin();v!=p_t->second.end();v++)
		{
			if(*v=="^")
			{
				//cout<<"ad:"<<p_t->first<<" "<<*v<<endl;
				rowNo=selectRow(p_t->first);
				for(int j=0;j<f[p_t->first].size();j++)
				{
					colNo=selectColumn(f[p_t->first][j]);
					if(table[rowNo][colNo]=="")
						table[rowNo][colNo]+=*v;
					else
					{
						fprintf(stderr,"FIRST-FOLLOW Conflict! Not a LL(1) grammar");
						exit(0);
					}
				}
			}
		}
	}
	cout<<"Table:"<<endl;
	for(int i=0;i<nt.size();i++)
	{
		for(int j=0;j<t.size();j++)
		{
			cout<<nt[i]<<","<<t[j]<<": ";
			cout<<table[i][j]<<endl;
		}
		cout<<endl;
	}
}

void findFirst()
{
	bool changing=true;
	int count=0,pre_count=-1;
	//Setting the first of terminals to terminals
	for(int i=0;i<t.size();i++)
	{
		m_term[t[i]]=t[i];
	}
	//construction of first set
	while(changing)
	{
		for(p_t=p.begin();p_t!=p.end();p_t++) // each NT's productions
		{
			for(v=p_t->second.begin();v!=p_t->second.end();v++)// each production of a particular NT
			{
				int i;
				char *token,*tmp;
				token=getTokens(*v," "); // gets first character of right hand side of production
				tmp=token;
				i=0;
				while(tmp!=0)
				{
					i++;
					tmp=strtok(NULL," ");	
				}
				int nooftokens=i;
				if(m_term[token]=="")	// matches a nonterminal
				{
					//check non terminal map for first set
					for(int k=0;k<m_nt[token].size();k++)
					{	// pushes first set of 
						if(m_nt[token][k]!="^")
						{
							if(!isThere(m_nt[p_t->first],m_nt[token][k]))
							{
		
								m_nt[p_t->first].push_back(m_nt[token][k]); 
								count++;
							}
						}
					}
				}
				else	// matches a terminal
				{
					if(token!="^")
					{
						if(!isThere(m_nt[p_t->first],token))
						{
		
							m_nt[p_t->first].push_back(token);
							count++;
						}
					}
				}
				i=1;
				tmp=token;
				while(i<nooftokens)
				{
					if(m_term[tmp]!="")
					{
						if(tmp!="^")
							break;
					}
					else
					{
						if(m_nt[tmp].size()!=0)
						{
							bool epsilon=false;
							//tmp=strtok(NULL," ");
							if(m_term[tmp]!="")
							{
								if(tmp=="^")
									epsilon=true;
							}
							else
							{
								for(int k=0;k<m_nt[tmp].size();k++)	// pushes first set of 
										if(m_nt[tmp][k]=="^")
											epsilon=true;
							}
							if(epsilon==false)
								break;
							if(m_term[tmp]=="")	// matches a nonterminal
							{
								//check non terminal map for first set
								for(int k=0;k<m_nt[tmp].size();k++)	// pushes first set of 
									if(m_nt[tmp][k]!="^")
									{
										if(!isThere(m_nt[p_t->first],m_nt[tmp][k]))
										{
		
											m_nt[p_t->first].push_back(m_nt[tmp][k]); 
											count++;
										}
									}
							}
							else	// matches a terminal
							{
								if(!isThere(m_nt[p_t->first],tmp))
								{
		
									m_nt[p_t->first].push_back(tmp); 
									count++;
								}
							}
						}
						else
							break;
					}
					i++;
					tmp=strtok(NULL," ");
				}
				//if cond
				if(i==nooftokens)
				{
					if(m_term[tmp]!="")
					{
						if(tmp=="^")
						{
							if(!isThere(m_nt[p_t->first],"^"))
							{
		
								m_nt[p_t->first].push_back("^"); 
								count++;
							}
						}	
					}
					else
					{
						for(int e=0;e<m_nt[tmp].size();e++)
						{
							if(m_nt[tmp][e]=="^")
							{
								if(!isThere(m_nt[p_t->first],"^"))
								{
		
									m_nt[p_t->first].push_back("^"); 
									count++;
									break;
								}
							}
						}
					}
				}
			}
		}
		if(pre_count==count)
		{
			changing=false;
		}
		else
			pre_count=count;
	}
}

int main()
{
	string str;
	ifstream fin;
	fin.open("input2.txt",ios::in);	
	getline(fin,str);
	nts=getTokens(str," ");
	cout<<"Non-Terminals:"<<endl;
	while(nts!=0)
	{
		nt.push_back(nts);
		cout<<nts<<" ";
		nts=strtok(NULL," ");
	}
	getline(fin,str);
	ts=getTokens(str," ");
	cout<<"\n\nTerminals:"<<endl;
	while(ts!=0)
	{
		t.push_back(ts);
		cout<<ts<<" ";
		ts=strtok(NULL," ");
	}
	//t.push_back("^");
	t.push_back("$");
	cout<<"^ $"<<endl;
	//cout<<"\nProductions"<<endl;
	getline(fin,str);
	while(str!="-")
	{
		prod.push_back(str);
		char *lhs,*rhs;
		vector<string> tmp;
		rhs=lhs=getTokens(str,":");
		rhs=strtok(NULL,"\n");
		p[lhs].push_back(rhs);
		getline(fin,str);
		//cout<<rhs<<" "<<lhs<<endl;
	}
	findFirst();
	cout<<"\n\nFirst set:"<<endl;
	for(m_t=m_nt.begin();m_t!=m_nt.end();m_t++)
	{
		cout<<m_t->first<<": ";
		for(v=m_t->second.begin();v!=m_t->second.end();v++)
		{
			cout<<*v<<" ";
		}
		cout<<endl;
	}
	findfollow();
	cout<<"\n\nFollow set:"<<endl;
	for(m_t=f.begin();m_t!=f.end();m_t++)
	{
	    cout<<m_t->first<<":";
	    for(v=m_t->second.begin();v!=m_t->second.end();v++)
	      cout<<*v<<" ";
	    cout<<endl;
	}
	parseTableConstruct();
	string inputString;
	vector <string>tmp;
	getline(fin,inputString);
	char *twmp=getTokens(inputString," ");
	while(twmp!=0)
	{
		tmp.push_back(twmp);
		twmp=strtok(NULL," \n");
	}
	parse(tmp);
	//cout<<m_term["Name"]<<endl;
	//if(m_term["id"]=="")
	//	cout<<"asdk";
	return 0;
}
