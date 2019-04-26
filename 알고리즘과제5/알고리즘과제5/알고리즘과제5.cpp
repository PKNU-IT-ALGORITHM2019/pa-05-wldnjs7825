#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct wordnode {
	char *wordname; //단어
	char *cla;		//품사
	char *exp;		//설명
	struct wordnode *left;
	struct wordnode *right;
	struct wordnode *parent;
}word;

word *search_tree(word *root, char find_data[]);
void delete_tree(char delete_data[]);
word *head = NULL;
word *tree_root = NULL;
word *newnode;
int w_cnt = 0;

void make_head(word *newnode)
{
	head = newnode;
	tree_root = newnode;
	head->left = NULL;
	head->right = NULL;
	head->parent = NULL;
}

void make_tree(word *root,word *newnode)
{
	int cmp = strcmp(newnode->wordname, root->wordname);
	if (cmp < 0)
	{
		if (root->left == NULL)
		{
			root->left = newnode;
			newnode->left = NULL;
			newnode->right = NULL;
			newnode->parent = root;
		}
		else
		{
			make_tree(root->left, newnode);
		}
	}
	else
	{
		if (root->right == NULL)
		{
			root->right = newnode;
			newnode->left = NULL;
			newnode->right = NULL;
			newnode->parent = root;
		}
		else
		{
			make_tree(root->right, newnode);
		}
	}	
}

void find_tree() {
	char find_data[20];
	printf("검색 할 단어를 입력하시오:");
	scanf_s("%s", find_data,sizeof(find_data));
	if (search_tree(tree_root, find_data) == NULL)
	{
		printf("찾으시는 단어가 없습니다.\n");
	}
	else
	{
		printf("%s\n", search_tree(tree_root, find_data)->exp);
	}
}

word *search_tree(word *root,char find_data[])
{
	int cmp = strcmp(find_data, root->wordname);
	if (cmp == 0)
		return root;
	else if (cmp < 0)
	{
		if (root->left == NULL)
			return NULL;
		return search_tree(root->left, find_data);
	}
	else
	{
		if (root->right == NULL)
			return NULL;
		return search_tree(root->right, find_data);
	}
}

void add()
{
	char temp[1000];
	newnode = (word *)malloc(sizeof(word));
	printf("추가 할 단어를 입력하시오\n");
	printf("word:");
	scanf_s("%s", temp, sizeof(temp));
	newnode->wordname = _strdup(temp);

	printf("class:");
	scanf_s("%s", temp, sizeof(temp));
	newnode->cla = _strdup(temp);
	
	printf("meaning:");
	scanf_s("%s", temp, sizeof(temp));
	newnode->exp = _strdup(temp);

	make_tree(tree_root, newnode);
	w_cnt++;

}

word *successor(word *newnode) //자식 노드 중 가장 큰값 리턴, 삭제할 노드 왼쪽 자식 노드를변수로 받음
{
	if (newnode->right == NULL)
		return newnode;
	return successor(newnode->right);
}

void Delete()
{
	char delete_data[20];
	printf("삭제 할 단어를 입력하시오:");
	scanf_s("%s", delete_data, sizeof(delete_data));
	delete_tree(delete_data);
}

void delete_tree(char delete_data[])
{	
	word *delete_word = NULL;
	if (search_tree(tree_root, delete_data) == NULL)
	{
		printf("삭제하려시는 단어가 없습니다.\n");
	}
	else
	{
		delete_word = search_tree(tree_root, delete_data);		
		int cmp = strcmp(delete_word->wordname, delete_word->parent->wordname);

		if(delete_word->left == NULL && delete_word->right == NULL)
		{
			if (cmp < 0)
				delete_word->parent->left = NULL;
			else 
				delete_word->parent->right = NULL;
			printf("%s 제거\n", delete_word->wordname);
			free(delete_word);
		}
		else if(delete_word->left != NULL && delete_word->right == NULL)
		{
			delete_word->left->parent = delete_word->parent;
			delete_word->parent->left = delete_word->left;
			printf("%s 제거\n", delete_word->wordname);
			free(delete_word);
		}
		else if(delete_word->left == NULL && delete_word->right != NULL)
		{
			delete_word->right->parent = delete_word->parent;
			delete_word->parent->right = delete_word->right;
			printf("%s 제거\n", delete_word->wordname);
			free(delete_word);
		}
		else
		{
			word * delete_suc = successor(delete_word->left);
			word * delete_left = delete_word->left;
			if (delete_left->left == NULL && delete_left->right == NULL)
			{
				delete_word->parent->left = delete_left;
				delete_left->right = delete_word->right;
				delete_word->right->parent = delete_left;
			}	
			else if (delete_suc->left == NULL)
			{
				delete_word->left->parent = delete_suc;
				delete_word->right->parent = delete_suc;

				if (cmp < 0)
				{
					delete_word->parent->left = delete_suc;
				}
				else
				{
					delete_word->parent->right = delete_suc;
				}
				delete_suc->parent = delete_word->parent;
				delete_suc->parent->right = NULL;
				delete_suc->parent->left = NULL;	
				printf("%s 제거\n", delete_word->wordname);
				free(delete_word);
			}
			else 
			{
				delete_word->left->parent = delete_suc;
				delete_word->right->parent = delete_suc;

				if (cmp < 0)
				{
					delete_word->parent->left = delete_suc;
				}
				else
				{
					delete_word->parent->right = delete_suc;
				}

				delete_suc->parent->right = delete_suc->left;
				delete_suc->left->parent = delete_suc->parent;

				delete_suc->left = delete_word->left;
				delete_suc->right = delete_word->right;
				delete_suc->parent = delete_word->parent;
				printf("%s 제거\n", delete_word->wordname);
				free(delete_word);
			}
		}
	}
}

void deleteall()
{
	FILE *fp;
	char *token;
	char *cont = NULL;
	char buffer[5000];
	int d_cnt = 0;

	fopen_s(&fp, "to_be_deleted_words.txt", "r");

	while (!feof(fp)) {
		fgets(buffer, sizeof(buffer), fp);
		token = strtok_s(buffer, "\n", &cont);
		
		delete_tree(token);
		d_cnt++;
		
		
	}
	printf("%d words were deleted successfully.\n", d_cnt);
	fclose(fp);
}

int main() {
	FILE *fp;
	char *token[3];
	char *cont = NULL;
	char buffer[5000];
	
	fopen_s(&fp, "shuffled_dict.txt", "r");
	
	while(!feof(fp)){
		newnode = (word *)malloc(sizeof(word));
		fgets(buffer, sizeof(buffer), fp);
		token[0] = strtok_s(buffer, " ", &cont);
		token[1] = strtok_s(cont, ")", &cont);
		token[2] = strtok_s(cont, "\n", &cont);

		newnode->wordname = _strdup(token[0]);
		newnode->cla = _strdup(token[1]);
		newnode->exp = _strdup(token[2]);
		if (head == NULL)
			make_head(newnode);
		else
			make_tree(tree_root, newnode);
		w_cnt++;
	}
	
	printf("저장된 단어의 개수 : %d\n", w_cnt); 
	find_tree();							 
	add();									
	Delete();	
	deleteall();

	fclose(fp);

	return 0;
}
