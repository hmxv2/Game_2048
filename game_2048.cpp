#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>

int xytoi(int x, int y, int rowlen);//����ά���������ת��Ϊһά���������
bool merge_every_row(std::vector<int>& v);//�Ծ���ÿһ�ж�����merge_num����
bool merge_num(std::vector<int>& v, int bi, int ei);//��һ������������λ���ƶ������ж��ƶ�֮���Ƿ���Խ��������ں�
bool game_over(const std::vector<int>& v);//��Ϸ�Ƿ����
void rotate_anti_clock(std::vector<int>& v);//��ʱ����ת90��
void print_grid(const std::vector<int>& v);//�������
void randomly_generate_new_num(std::vector<int>& v);//����ڿ�λ������һ������2

bool move_left(std::vector<int>& v);//����
bool move_up(std::vector<int>& v);//����
bool move_right(std::vector<int>& v);//����
bool move_down(std::vector<int>& v);//����
bool if_matrix_equal(const std::vector<int>& v1, const std::vector<int>& v2);//�����ȴ�С�ľ����Ƿ����
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
    }//����ļ����ܴ򿪣���ȫ�����0��ĩλ��Ϊ2
    else{
        int n;
        while(infile >> n){
            in.push_back(n);
        }
    }//��������ļ�������

    //cout<<"in size: "<<in.size()<<endl;//for debug
	
	print_grid(in);//�������
	//��ʼ��Ϸ
    string action_input;
    while(!game_over(in)){
    
        cin >> action_input;
        //bool action_input_available=0;//����Ķ����Ƿ����ʹ�þ������仯�����ֱ仯�������֣�1�����������λ�ñ䶯��û���ںϣ�2������λ�ñ䶯�����������ں�
        vector<int> in_backup(in);//����
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
        //����ִ����ϣ��ж϶����Ƿ���Ч�����жϾ����Ƿ��б仯
		if(!if_matrix_equal(in,in_backup))//������Ч
		{
			randomly_generate_new_num(in);//����ڿ�λ����һ��2
			print_grid(in);//�������
		}


    }//���game over=0����Ϸ����
    
    cout << "game over" << endl;
    

	system("pause");//�е�IDE���ܲ���Ҫ��һ��
	return 0;
}

int xytoi(int x, int y, int rowlen){
    return x*rowlen+y;
}//��ά�����������ת��Ϊһά���е�����


void print_grid(const std::vector<int>& v){
    int rowlen = std::sqrt(v.size());
    
    for(int i = 0; i < rowlen; i++){
        for(int j = 0; j < rowlen; j++){
			//Ϊ�����׿���0�Ͳ�����ˣ����һ���ո�
			if(v[xytoi(i,j,rowlen)]==0) cout<<' '<<"\t"; else
				cout << v[xytoi(i,j,rowlen)] << "\t";
        }
        cout << endl;
    }
}//�������

//��������ʱ����ת90��
void rotate_anti_clock(std::vector<int>& v){
    std::vector<int> tmp;
    int size=std::sqrt(v.size());
    //������ĿҪ��ʹ��xytoi��������ȷ���׶���
	for(int col=size-1;col>=0;col--)
	{
		for(int row=0;row<size;row++)
		{
			tmp.push_back(v[xytoi(row,col,size)]);
		}
	}
	for(int i=0;i<v.size();i++)
		v[i]=tmp[i];
}//��ʱ����ת����


/*˼·��
Ϊ�˱����4�����򶼱�д�ںϺ�����������ͳһ����˼·���£�
ȷ��һ�������򣬱����������ǵĺ�������ֻ���Ǵ������������������ϣ����º�����
�����ȫ��ʹ����ת������ת��������������ͳһ�����ж������ں�����ں���
����ת��֮ǰ�ķ���*/
//��Ϊ���ǵ������ȡ���㣬ѡȡ����Ϊ���ǵ������򣬹ʸ��ںϺ���ֻ��������ȫ������ѹ���������ܷ������ںϡ�
bool merge_every_row(std::vector<int>& v)
{
	int rowlen=sqrt(v.size());//����
	int col_len=rowlen;//����==����
	int merge_rows_count=0;//��¼�ж������ںϳɹ�
	for(int i=0;i<rowlen;i++)
	{
		//cout<<"in function merge_every_row()�� "<<i*col_len<<' '<<i*col_len+col_len<<endl;//for debug
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
//ÿһ�ж�����ѹ������λ�ÿ��ܷ����仯���ҿ����������������ں�
bool merge_num(std::vector<int>& v, int bi, int ei)//һ�е����ܷ������ں�
{
	//�Ƚ�bi��ei֮���0Ԫ��ȥ����������Ԫ�ض�������࣬ʣ�ಹ0
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
	//ĩβ����zero_count��0Ԫ��
	for(int i=0;i<zero_count;i++)
	{
		one_row.push_back(0);
	}
	//cout<<"in function merge_num()�� "<<one_row.size()<<' '<<endl;//for debug

	//��ʼ����Ƿ��ܽ��������ں�
	bool merged_flag=0;//��־���Ƿ���й��ں�
    vector<int> merge_result;
	one_row.push_back(0);//��0��֤���һ�����������ܱ�����merge_result
	for(int i=0;i<one_row.size()-1;i++)
	{
		if((one_row[i]==one_row[i+1])&&(one_row[i]!=0))
		{
			merged_flag=1;//���й��ںϣ���¼����
			merge_result.push_back(one_row[i]+one_row[i+1]);//�ںϲ�д��merge_result����
			i++;//��һ�������ں��ˣ�����
		}
		else
			merge_result.push_back(one_row[i]);
	}

	//cout<<"in function merge_num()�� "<<one_row.size()-1<<' '<<merge_result.size()<<endl;
	//���ںϵĽ��merge_resultĩβ��0
	int merge_result_size=merge_result.size();
	for(int i=0;i<one_row.size()-1-merge_result_size;i++)//�˴�����one_row�ĳ�����Ҫ-1
		merge_result.push_back(0);

	//cout<<"in function merge_num()�� ";for(int i=0;i<merge_result.size();i++)cout<<merge_result[i]<<' ';cout<<endl;//for debug

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
    }//0Ԫ�ص�����
	//�ж��Ƿ��п�λ
	if(zero_nums_index.size()!=0)//�п�λ
	{
		srand((unsigned)time(NULL));//srand()��������һ���Ե�ǰʱ�俪ʼ���������
		int target_index=rand()%zero_nums_index.size();//Լ������ֵ������[0,tmp.size()-1]��zero_nums_index[target_index]����v��Ҫ������Ϊ2���Ǹ������±�
		v[zero_nums_index[target_index]]=2;//����Ŀֻ�趨Ϊ2
	}
}


bool game_over(const std::vector<int>& v)//����1����Ϸ����
{
	//��Ϸ����������ǣ�����û�п�λ�����ĸ������޷����������ں�
	//�жϾ������޿�λ
	for(int i=0;i<v.size();i++)
	{
		if(v[i]==0)
			return 0;
	}
	//û��������ѭ���б�return˵�������Ѿ����ˣ��������ж��ܷ��ںϣ���������Ϸ����
    vector<int> v_backup1(v);//v�ı���1
	vector<int> v_backup2(v);//v�ı���2
	vector<int> v_backup3(v);//v�ı���3
	vector<int> v_backup4(v);//v�ı���4
	//move������ı�v��ֵ���ʽ���4�ݱ��ݣ������ݴ����move����
	if(move_left(v_backup1)||move_up(v_backup2)||move_right(v_backup3)||move_down(v_backup4))//4��������һ�����ں�����Ϸδ����
		return 0;
	else
		return 1;

}//�����Ϸ�Ƿ����
//����
bool move_left(std::vector<int>& v)
{
	return merge_every_row(v);
}
//���ϣ��Ⱦ���һ����ʱ����ת��ת��Ϊ����ʹ������ĺ��������ںϣ�֮���ٽ������ʱ����ת3�Σ���Ч��ֱ������
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
//���ң��Ⱦ���2����ʱ����ת��ת��Ϊ����ʹ������ĺ��������ںϣ�֮���ٽ������ʱ����ת2�Σ���Ч��ֱ������
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
//���£��Ⱦ���3����ʱ����ת��ת��Ϊ����ʹ������ĺ��������ںϣ�֮���ٽ������ʱ����ת1�Σ���Ч��ֱ������
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
		all_element_equal=all_element_equal&&(v1[i]==v2[i]);//ֻҪ��һ��Ԫ�ز������all_element_equalΪ0
	}
	return all_element_equal;
}