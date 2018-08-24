#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>

int xytoi(int x, int y, int rowlen);//将二维矩阵的索引转化为一维数组的索引
bool merge_every_row(std::vector<int>& v);//对矩阵每一行都调用merge_num函数
bool merge_num(std::vector<int>& v, int bi, int ei);//对一行向左做数字位置移动，并判断移动之后是否可以进行数字融合
bool game_over(const std::vector<int>& v);//游戏是否结束
void rotate_anti_clock(std::vector<int>& v);//逆时针旋转90度
void print_grid(const std::vector<int>& v);//输出矩阵
void randomly_generate_new_num(std::vector<int>& v);//随机在空位处产生一个数字2

bool move_left(std::vector<int>& v);//向左
bool move_up(std::vector<int>& v);//向上
bool move_right(std::vector<int>& v);//向右
bool move_down(std::vector<int>& v);//向下
bool if_matrix_equal(const std::vector<int>& v1, const std::vector<int>& v2);//两个等大小的矩阵是否相等
using namespace std;

int main()
{
	
    vector<int> in;
    ifstream infile;
    string name;
    
    cout << "enter initial configuration file name: " << endl;
    cin >> name;
    
    infile.open(name.c_str());
    
    if(!infile.is_open()){
        cout << "file not found, using default start configuration" << endl;
        for(int i=0; i<15; i++){
            in.push_back(0);
        }
        in.push_back(2);
    }//如果文件不能打开，就全部输出0，末位数为2
    else{
        int n;
        while(infile >> n){
            in.push_back(n);
        }
    }//否则输出文件的数据

    //cout<<"in size: "<<in.size()<<endl;//for debug
	
	print_grid(in);//输出矩阵
	//开始游戏
    string action_input;
    while(!game_over(in)){
    
        cin >> action_input;
        //bool action_input_available=0;//输入的动作是否可以使得矩阵发生变化，这种变化包括两种：1，矩阵的数字位置变动但没有融合；2，数字位置变动并且有数字融合
        vector<int> in_backup(in);//备份
		if(action_input=="w"){
            move_up(in);
        }
        if(action_input=="s"){
            move_down(in);
        }
        if(action_input=="a"){
            move_left(in);
        }
        if(action_input=="d"){
            move_right(in);
        }
        //动作执行完毕，判断动作是否有效，即判断矩阵是否有变化
		if(!if_matrix_equal(in,in_backup))//动作有效
		{
			randomly_generate_new_num(in);//随机在空位处加一个2
			print_grid(in);//输出矩阵
		}


    }//如果game over=0，游戏继续
    
    cout << "game over" << endl;
    

	system("pause");//有的IDE可能不需要这一行
	return 0;
}

int xytoi(int x, int y, int rowlen){
    return x*rowlen+y;
}//二维矩阵的索引，转化为一维数列的索引


void print_grid(const std::vector<int>& v){
    int rowlen = std::sqrt(v.size());
    
    for(int i = 0; i < rowlen; i++){
        for(int j = 0; j < rowlen; j++){
			//为了容易看，0就不输出了，输出一个空格
			if(v[xytoi(i,j,rowlen)]==0) cout<<' '<<"\t"; else
				cout << v[xytoi(i,j,rowlen)] << "\t";
        }
        cout << endl;
    }
}//输出矩阵

//将矩阵逆时针旋转90度
void rotate_anti_clock(std::vector<int>& v){
    std::vector<int> tmp;
    int size=std::sqrt(v.size());
    //按照题目要求使用xytoi函数，以确保易读性
	for(int col=size-1;col>=0;col--)
	{
		for(int row=0;row<size;row++)
		{
			tmp.push_back(v[xytoi(row,col,size)]);
		}
	}
	for(int i=0;i<v.size();i++)
		v[i]=tmp[i];
}//逆时针旋转矩阵


/*思路：
为了避免对4个方向都编写融合函数，考虑做统一处理，思路如下：
确立一个主方向，比如向左，我们的函数处理只考虑处理向左的命令，其余向上，向下和向右
的命令，全部使用旋转函数旋转成向左的情况，做统一处理，判断数字融合与否，融合完
再旋转回之前的方向*/
//因为考虑到数组读取方便，选取向左为我们的主方向，故该融合函数只考虑数字全部向左压，看数字能否向左融合。
bool merge_every_row(std::vector<int>& v)
{
	int rowlen=sqrt(v.size());//行数
	int col_len=rowlen;//行数==列数
	int merge_rows_count=0;//记录有多少行融合成功
	for(int i=0;i<rowlen;i++)
	{
		//cout<<"in function merge_every_row()： "<<i*col_len<<' '<<i*col_len+col_len<<endl;//for debug
		if(merge_num(v,i*col_len,i*col_len+col_len))
		{
			merge_rows_count++;
		}
	}
	if(merge_rows_count==0)
		return 0;
	else
		return 1;

}
//每一行都向左压，数字位置可能发生变化并且可能有数字能向左融合
bool merge_num(std::vector<int>& v, int bi, int ei)//一行的数能否向左融合
{
	//先将bi到ei之间的0元素去除，即非零元素都在最左侧，剩余补0
    vector<int> one_row;
	int rowlen=ei-bi;
	int zero_count=0;
	for(int i=bi;i<ei;i++)
	{
		if(v[i]!=0)
			one_row.push_back(v[i]);
		else
			zero_count++;
	}
	//末尾加入zero_count个0元素
	for(int i=0;i<zero_count;i++)
	{
		one_row.push_back(0);
	}
	//cout<<"in function merge_num()： "<<one_row.size()<<' '<<endl;//for debug

	//开始检查是否能进行向左融合
	bool merged_flag=0;//标志，是否进行过融合
    vector<int> merge_result;
	one_row.push_back(0);//加0保证最后一个非零数字能被加入merge_result
	for(int i=0;i<one_row.size()-1;i++)
	{
		if((one_row[i]==one_row[i+1])&&(one_row[i]!=0))
		{
			merged_flag=1;//进行过融合，记录下来
			merge_result.push_back(one_row[i]+one_row[i+1]);//融合并写入merge_result数组
			i++;//后一个数被融合了，跳过
		}
		else
			merge_result.push_back(one_row[i]);
	}

	//cout<<"in function merge_num()： "<<one_row.size()-1<<' '<<merge_result.size()<<endl;
	//对融合的结果merge_result末尾补0
	int merge_result_size=merge_result.size();
	for(int i=0;i<one_row.size()-1-merge_result_size;i++)//此处计算one_row的长度需要-1
		merge_result.push_back(0);

	//cout<<"in function merge_num()： ";for(int i=0;i<merge_result.size();i++)cout<<merge_result[i]<<' ';cout<<endl;//for debug

	if(merged_flag==1)
	{
		for(int i=bi;i<ei;i++)
			v[i]=merge_result[i-bi];
		return 1;
	}
	else
	{
		for(int i=bi;i<ei;i++)
			v[i]=one_row[i-bi];
		return 0;
	}
}


void randomly_generate_new_num(std::vector<int>& v){
    vector<int> zero_nums_index;
    for(int i=0; i<v.size(); i++){
        if(v[i]==0){
            zero_nums_index.push_back(i);
        }
    }//0元素的索引
	//判断是否有空位
	if(zero_nums_index.size()!=0)//有空位
	{
		srand((unsigned)time(NULL));//srand()函数产生一个以当前时间开始的随机种子
		int target_index=rand()%zero_nums_index.size();//约束索引值在区间[0,tmp.size()-1]，zero_nums_index[target_index]即是v中要被设置为2的那个数的下标
		v[zero_nums_index[target_index]]=2;//此题目只设定为2
	}
}


bool game_over(const std::vector<int>& v)//返回1则游戏结束
{
	//游戏结束的情况是：矩阵没有空位而且四个方向都无法进行数字融合
	//判断矩阵有无空位
	for(int i=0;i<v.size();i++)
	{
		if(v[i]==0)
			return 0;
	}
	//没有在上面循环中被return说明矩阵已经满了，接下来判断能否融合，不能则游戏结束
    vector<int> v_backup1(v);//v的备份1
	vector<int> v_backup2(v);//v的备份2
	vector<int> v_backup3(v);//v的备份3
	vector<int> v_backup4(v);//v的备份4
	//move函数会改变v的值，故进行4份备份，将备份传输进move函数
	if(move_left(v_backup1)||move_up(v_backup2)||move_right(v_backup3)||move_down(v_backup4))//4个方向有一个可融合则游戏未结束
		return 0;
	else
		return 1;

}//检测游戏是否结束
//向左
bool move_left(std::vector<int>& v)
{
	return merge_every_row(v);
}
//向上，先经过一次逆时针旋转，转化为向左，使用向左的函数进行融合，之后再将结果逆时针旋转3次，等效于直接向上
bool move_up(std::vector<int>& v)
{
	//
	rotate_anti_clock(v);
	//
	bool merged_flag=merge_every_row(v);
	//
	rotate_anti_clock(v);
	rotate_anti_clock(v);
	rotate_anti_clock(v);
	return merged_flag;
}
//向右，先经过2次逆时针旋转，转化为向左，使用向左的函数进行融合，之后再将结果逆时针旋转2次，等效于直接向右
bool move_right(std::vector<int>& v)
{
	//
	rotate_anti_clock(v);
	rotate_anti_clock(v);
	//
	bool merged_flag=merge_every_row(v);
	//
	rotate_anti_clock(v);
	rotate_anti_clock(v);

	return merged_flag;
}
//向下，先经过3次逆时针旋转，转化为向左，使用向左的函数进行融合，之后再将结果逆时针旋转1次，等效于直接向下
bool move_down(std::vector<int>& v)
{
	//
	rotate_anti_clock(v);
	rotate_anti_clock(v);
	rotate_anti_clock(v);
	//
	bool merged_flag=merge_every_row(v);
	//
	rotate_anti_clock(v);

	return merged_flag;
}

bool if_matrix_equal(const std::vector<int>& v1, const std::vector<int>& v2)
{
	bool all_element_equal=1;
	for(int i=0;i<v1.size();i++)
	{
		all_element_equal=all_element_equal&&(v1[i]==v2[i]);//只要有一个元素不相等则all_element_equal为0
	}
	return all_element_equal;
}