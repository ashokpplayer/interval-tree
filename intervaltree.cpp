#include<iostream>
#include<algorithm>
#include<math.h>
#include<cstring>
#include<iomanip>
#include<stdio.h>
#define all(v) (v.begin(),v.end())
using namespace std;
bool flag=0;
struct node
{
    pair<int,int> p;//contains the range
    int big;//maximum upper bound of all subtree routed at node
    bool colour;//color of the node
    node *left,*right,*father;//the left child, right child and the parent nodes
};
void upgrade(node *root)//upgrading the maximum value with that as root
{
    if(root==NULL)
        return;
    upgrade(root->left);
    upgrade(root->right);
    if(root->left!=NULL&&root->right!=NULL)
        root->big=max(root->big,max(root->left->big,root->right->big));
    else if(root->left!=NULL)
        root->big=max(root->big,root->left->big);
    else if(root->right!=NULL)
        root->big=max(root->big,root->right->big);
    return;
}

node *insert_node(node *root,node *now_)//inserting the node
{
    if(root==NULL)
        return now_;

    root->big=max(root->big,now_->big);
    if(now_->p.first<root->p.first)
    {
        root->left=insert_node(root->left,now_);
        root->left->father=root;
    }
    else
    {
        root->right=insert_node(root->right,now_);
        root->right->father=root;
    }
    return root;

}
void rleft(node *&now_,node *&root)//left rotation
{
    node *right=now_->right;
    now_->right=right->left;

    if(now_->right!=NULL)
        now_->right->father=now_;
    right->father=now_->father;
    if(now_->father==NULL)
        root=right;
    else if(now_==now_->father->left)
        now_->father->left=right;
    else
        now_->father->right=right;
    right->left=now_;
    now_->father=right;
}
void rright(node *&now_,node *&root)//right rotation
{
    node *left=now_->left;
    now_->left=left->right;
    if(now_->left!=NULL)
        now_->left->father=now_;
    left->father=now_->father;
    if(now_->father==NULL)
        root=left;
    else if(now_==now_->father->left)
        now_->father->left=left;
    else
        now_->father->right=left;
    left->right=now_;
    now_->father=left;
}
void refresh(node *&root,node *&now_)//correting the red black tree
{
    node *father=NULL;
    node *g_p=NULL;
    while(now_!=root&&now_->colour==0&&now_->father->colour==0)
    {
        father=now_->father;
        g_p=now_->father->father;
        if(g_p->left==father)
        {
            node *uncl=g_p->right;
            if(uncl!=NULL&&uncl->colour==0)
            {
                g_p->colour=0;
                father->colour=1;
                uncl->colour=1;
                now_=g_p;
            }
            else
            {
                if(now_==father->right)
                {
                    rleft(now_,root);
                    now_=father;
                    father=now_->father;
                }
                rright(g_p,root);
                swap(father->colour,g_p->colour);
                now_=father;
            }
        }
        else
        {
            node *uncl=g_p->left;
            if(uncl!=NULL&&uncl->colour!=1)
            {
                g_p->colour=0;
                father->colour=1;
                uncl->colour=1;
                now_=g_p;
            }
            else
            {
                if(now_==father->left)
                {
                    rright(father,root);
                    now_=father;
                    father=now_->father;
                }
                rleft(g_p,root);
                swap(g_p->colour,father->colour);
                now_=father;
            }
        }
    }
    root->colour=1;

}

void print(node *root)//printing the in-order traversal
{
    if(root==NULL)
        return;
    print(root->left);
    cout<<root->p.first<<" "<<root->p.second<<" "<<root->big<<" "<<root->colour<<endl;
    print(root->right);
}
node *next_node(node* cur)
{
    node* temp;
    if(cur->right!=NULL)
    {
        while(cur->left)
            cur=cur->left;
        return cur;
    }
    temp=cur->father;
    while(temp!=NULL && cur==temp->right)
    {
        cur=temp;
        temp=temp->father;
    }
    return temp;
}
node *exact_find(node *root,pair<int,int> p)
{
    if(p.first==root->p.first&&p.second==root->p.second)
        return root;
    if(root->left!=NULL&&root->left->big>=p.first)
        return exact_find(root->left,p);
    else
        return exact_find(root->right,p);
}
void search_interval_tree(node *root,pair<int,int> p)//searching a range
{
    if(root==NULL)
        return;
    int l=max(root->p.first,p.first),r=min(root->p.second,p.second);
    if(l<=r)
    {
        cout<<"range is ("<<root->p.first<<" "<<root->p.second<<")"<<endl;
        flag=1;
        return;
    }
    if(root->left!=NULL&&root->left->big>=p.first)
        search_interval_tree(root->left,p);
    else
        search_interval_tree(root->right,p);
}
void delete_node(node *root,pair<int,int> p)//deletes a node
{
    node *now_=exact_find(root,p);
    node *first_temp=new node,*second_t=new node;
    
    if(now_->left==NULL || now_->right==NULL)
        first_temp=now_;
    else
        first_temp=next_node(now_);
    

    if(first_temp->left!=NULL)
        second_t=first_temp->left;
    else
        second_t=first_temp->right;


    second_t->father=first_temp->father;
    if(first_temp->father==NULL)
        root=second_t;
    else if(first_temp==first_temp->father->left)
    {
        first_temp->father->left=second_t;
        
    }
    else
        first_temp->father->right=second_t;
    
    if(first_temp!=now_)
    {
        now_->p.first=first_temp->p.first;
    }
    
    if(first_temp->colour==1)
        refresh(root,second_t);
    
}

int main()
{
    int n;
    cout<<"enter the number of nodes\n";
    cin>>n;
    cout<<"input the ranges\n";
    pair<int,int>a;
    node *root=NULL;
    for(int i=0;i<n;i++)
    {
        cin>>a.first>>a.second;
        node *now_=new node;
        now_->colour=0;
        now_->p=a;
        now_->big=a.second;
        now_->left=now_->right=now_->father=NULL;
        root=insert_node(root,now_);
        refresh(root,now_);
        upgrade(root);
    }
    cout<<"tree traversal\n";
    print(root);
    int q;
    cout<<"number of input?\n";
    cin>>q;
    cout<<"enter your ranges to search\n";
    pair<int,int>p;
    while(q--)
    {
        cin>>p.first>>p.second;
        flag=0;
        search_interval_tree(root,p);
        if(!flag)
            cout<<"interval not found"<<endl;
    }
//    cout<<"enter the node to delete\n";
//    cin>>a.first>>a.second;
//    delete_node(root,a);
//    correct(root);
//    cout<<"node deleted\n";
    return 0;
}

