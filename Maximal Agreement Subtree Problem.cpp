#include <iostream>
#include <string>

using namespace std;

class Node
{
public:
    Node* parent;
    Node* brother;
    Node* son;

    Node(Node* parent, int data)
    {
        this->data = data;
        this->parent = parent;
        this->brother = NULL;
        this->son = NULL;
    }


    int getData() const {
        return data;
    }

private:
    int data;
};

class Tree
{
public:
    int key;

    Tree()
    {
        root = new Node(NULL, NULL);
        currentNode = root;
        size = 1;
        leavesAmount = 0;
        nodesAmount = 1;
        key = -1;
    }
    int getSize() const { return size; }
    int getleavesAmount() const { return leavesAmount; }
    int getnodesAmount() const { return nodesAmount; }

    void addChild(int data)
    {
        if (root->son == NULL)
        {
            Node* newNode = new Node(root, data);
            root->son = newNode;
            currentNode = newNode;
            size++;
        }
        else
        {
            Node* newNode = new Node(currentNode, data);
            currentNode->son = newNode;
            currentNode = newNode;
            size++;
        }
        if (data > 0)
            leavesAmount++;
        else
            nodesAmount++;
    }
    void addBrother(int data)
    {
        Node* newNode = new Node(currentNode->parent, data);
        currentNode->brother = newNode;
        currentNode = newNode;
        size++;
        if (data > 0)
            leavesAmount++;
        else
            nodesAmount++;
    }
    void backToParent()
    {
        currentNode = currentNode->parent;
    }
    Node* getLeafbyIndex(int index) const
    {
        Node* temp = root;
        int counter = 0;
        while (counter <= index)
        {
            if (temp->son)
                temp = temp->son;
            else if (temp->brother)
                temp = temp->brother;
            else
            {
                while (temp->brother == NULL && temp->parent)
                    temp = temp->parent;
                if (temp->brother)
                    temp = temp->brother;
            }

            if (temp->getData() > 0)
                counter++;
        }
        return temp;
    }
    Node* getNodebyIndex(const int index) const
    {
        Node* temp = root;
        int counter = 0;
        while (counter < index)
        {
            if (temp->son)
                temp = temp->son;
            else if (temp->brother)
                temp = temp->brother;
            else
            {
                while (temp->brother == NULL && temp->parent)
                    temp = temp->parent;
                if (temp->brother)
                    temp = temp->brother;
            }

            if (temp->getData() < 0)
                counter++;
        }
        return temp;
    }
    int directChildrenAmount(int nodeIndex) const
    {
        Node* temp = getNodebyIndex(nodeIndex);
        int amount = 0;
        if (temp->son)
        {
            temp = temp->son;
            amount = 1;
            while (temp->brother != NULL)
            {
                temp = temp->brother;
                amount++;
            }
        }
        return amount;
    }
    int* getDirectChildrenValues(int nodeIndex)
    {
        Node* temp = getNodebyIndex(nodeIndex);
        int tabSize = directChildrenAmount(nodeIndex);
        int* tab = new int[tabSize];
        if (temp->son)
        {
            temp = temp->son;
            tab[0] = temp->getData();
        }
        for (int i = 1; i < tabSize; i++)
        {
            temp = temp->brother;
            tab[i] = temp->getData();
        }
        return tab;
    }
    ~Tree()
    {
        delete (root);
    }
private:
    int size;
    int leavesAmount;
    int nodesAmount;
    Node* root;
    Node* currentNode;
};

double toInt(string tempString)
{
    double tempInt = 0;
    int len = tempString.length();
    for (int i = 0; i <= len - 1; i++)
    {
        tempInt += (double(tempString[len - 1 - i]) - 48) * pow(10, i);
    }
    return tempInt;
}

class NodeProperties
{
public:
    int* children;
    NodeProperties()
    {
        this->value = 0;
        this->size = 0;
        this->children = NULL;
    }
    explicit NodeProperties(int value)
    {
        this->value = value;
        this->size = 0;
        this->children = NULL;
    }
    NodeProperties(int value, int size, int* children)
    {
        this->value = value;
        this->size = size;
        this->children = children;
    }

    int getValue() const { return value; }
    int getSize() const { return size; }

private:
    int value;
    int size;
};

NodeProperties* createNodeTab(Tree* tree)
{
    int treeSize = tree->getSize();
    int leavesAmount = tree->getleavesAmount();
    int nodesAmount = tree->getnodesAmount();
    NodeProperties* nodeTab = new NodeProperties[treeSize];
    for (int i = 0; i < leavesAmount; i++)
    {
        nodeTab[i] = NodeProperties(tree->getLeafbyIndex(i)->getData());
    }
    for (int i = 0; i < nodesAmount; i++)
    {
        nodeTab[leavesAmount + i] = NodeProperties(tree->getNodebyIndex(i)->getData(), tree->directChildrenAmount(i), tree->getDirectChildrenValues(i));
    }
    return nodeTab;
}

bool perm_moveTemp(int* temp, bool* flags, int height, int width)
{
    for (int i = temp[width - 1] + 1; i <= height - 1; i++)
    {
        if (flags[i])
        {
            flags[temp[width - 1]] = true;
            temp[width - 1] = i;
            flags[i] = false;
            return true;
        }
    }
    flags[temp[width - 1]] = true;
    if (width > 1)
    {
        if (perm_moveTemp(temp, flags, height, width - 1) == false)
        {
            return false;
        }
    }
    else
        return false;
    for (int i = 0; i < height; i++)
    {
        if (flags[i])
        {
            temp[width - 1] = i;
            flags[i] = false;
            return true;
        }
    }

    return false;
}

int perm(int** tab, int height, int width, int* temp, bool* flags, int maxi)
{
    do {
        int sum = 0;

        for (int i = 0; i < width; i++)
        {
            sum += tab[temp[i]][i];
        }
        if (sum > maxi)
            maxi = sum;
    } while (perm_moveTemp(temp, flags, height, width));
    return maxi;
}

int biggestPermutation(int** tab, int height, int width)
{
    int maxi = 0;
    int* temp = new int[width];
    bool* flags = new bool[height];
    for (int i = 0; i < width; i++)
        temp[i] = i;
    for (int i = 0; i < height; i++)
    {
        if (i < width)
            flags[i] = false;
        else
            flags[i] = true;
    }

    if (width > height)
    {
        int** tab2 = new int* [width];
        for (int i = 0; i < width; i++)
            tab2[i] = new int[height];
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
                tab2[i][j] = tab[j][i];
        }
        maxi = perm(tab2, width, height, temp, flags, maxi);
        delete[](tab2);
    }
    else
        maxi = perm(tab, height, width, temp, flags, maxi);

    return maxi;
}

int** compareTrees(Tree* tree1, Tree* tree2, NodeProperties* nodeTab1, NodeProperties* nodeTab2)
{
    int size1 = tree1->getSize();
    int size2 = tree2->getSize();
    int leavesAmount1 = tree1->getleavesAmount();
    int leavesAmount2 = tree2->getleavesAmount();

    int** tab = new int* [size1];
    for (int i = 0; i < size1; i++)
    {
        tab[i] = new int[size2];
    }

    for (int i = 0; i < size1; i++)
    {
        for (int j = 0; j < size2; j++)
        {
            tab[i][j] = 0;
        }
    }
    //leaves x leaves
    for (int i = 0; i < leavesAmount1; i++)
    {
        for (int j = 0; j < leavesAmount2; j++)
        {
            if (nodeTab1[i].getValue() == nodeTab2[j].getValue())
                tab[i][j] = 1;
            else
                tab[i][j] = 0;
        }
    }
    //nodes x leaves
    for (int i = size1 - 1; i >= leavesAmount1; i--)
    {
        for (int j = 0; j < leavesAmount2; j++)
        {
            for (int x = 0; x < nodeTab1[i].getSize(); x++)
            {
                if (nodeTab2[j].getValue() == nodeTab1[i].children[x])
                {
                    tab[i][j] = 1;
                    break;
                }
                else if (nodeTab1[i].children[x] <= 0)
                {
                    tab[i][j] = tab[i + (nodeTab1[i].getValue() - nodeTab1[i].children[x])][j];
                    if (tab[i][j] == 1)
                        break;
                }
            }
        }
    }
    //leaves x nodes 
    for (int j = size2 - 1; j >= leavesAmount2; j--)
    {
        for (int i = 0; i < leavesAmount1; i++)
        {
            for (int x = 0; x < nodeTab2[j].getSize(); x++)
            {
                if (nodeTab1[i].getValue() == nodeTab2[j].children[x])
                {
                    tab[i][j] = 1;
                    break;
                }
                else if (nodeTab2[j].children[x] <= 0)
                {
                    tab[i][j] = tab[i][j + (nodeTab2[j].getValue() - nodeTab2[j].children[x])];
                    if (tab[i][j] == 1)
                        break;
                }
            }
        }
    }
    //nodes x nodes
    int maximalAgreement = 0;
    for (int i = size1 - 1; i >= leavesAmount1; i--)
    {
        int** tempTab = new int* [nodeTab1[i].getSize()];

        for (int j = size2 - 1; j >= leavesAmount2; j--)
        {
            for (int k = 0; k < nodeTab1[i].getSize(); k++)
                tempTab[k] = new int[nodeTab2[j].getSize()];

            for (int x = 0; x < nodeTab1[i].getSize(); x++)
            {
                for (int y = 0; y < nodeTab2[j].getSize(); y++)
                {
                    if (nodeTab1[i].children[x] > 0 && nodeTab2[j].children[y] > 0)
                    {
                        if (nodeTab1[i].children[x] == nodeTab2[j].children[y])
                            tempTab[x][y] = 1;
                        else
                            tempTab[x][y] = 0;
                    }
                    else if (nodeTab1[i].children[x] <= 0 && nodeTab2[j].children[y] > 0)
                    {
                        for (int z = 0; z < leavesAmount2; z++)
                        {
                            if (nodeTab2[z].getValue() == nodeTab2[j].children[y])
                            {
                                tempTab[x][y] = tab[i + (nodeTab1[i].getValue() - nodeTab1[i].children[x])][z];
                                break;
                            }
                        }
                    }
                    else if (nodeTab1[i].children[x] > 0 && nodeTab2[j].children[y] <= 0)
                    {
                        for (int z = 0; z < leavesAmount1; z++)
                        {
                            if (nodeTab1[z].getValue() == nodeTab1[i].children[x])
                            {
                                tempTab[x][y] = tab[z][j + (nodeTab2[j].getValue() - nodeTab2[j].children[y])];
                                break;
                            }
                        }
                    }
                    else
                    {
                        tempTab[x][y] = tab[i + (nodeTab1[i].getValue() - nodeTab1[i].children[x])][j + (nodeTab2[j].getValue() - nodeTab2[j].children[y])];
                    }
                }
            }

            maximalAgreement = biggestPermutation(tempTab, nodeTab1[i].getSize(), nodeTab2[j].getSize());

            //sprawdzanie węzłów z dziećmi drugiego
            for (int x = 0; x < nodeTab2[j].getSize(); x++)
            {
                if (nodeTab2[j].children[x] > 0)
                {
                    if (maximalAgreement < 1)
                    {
                        for (int z = 0; z < leavesAmount2; z++)
                        {
                            if (nodeTab2[z].getValue() == nodeTab2[j].children[x])
                            {
                                if (tab[i][z] > maximalAgreement)
                                    maximalAgreement = tab[i][z];
                                break;
                            }
                        }
                    }
                }
                else
                {
                    if (tab[i][j + (nodeTab2[j].getValue() - nodeTab2[j].children[x])] > maximalAgreement)
                        maximalAgreement = tab[i][j + (nodeTab2[j].getValue() - nodeTab2[j].children[x])];
                }
            }
            for (int y = 0; y < nodeTab1[i].getSize(); y++)
            {
                if (nodeTab1[i].children[y] > 0)
                {
                    if (maximalAgreement < 1)
                    {
                        for (int z = 0; z < leavesAmount1; z++)
                        {
                            if (nodeTab1[z].getValue() == nodeTab1[i].children[y])
                            {
                                if (tab[z][j] > maximalAgreement)
                                    maximalAgreement = tab[z][j];
                                break;
                            }
                        }
                    }
                }
                else
                {
                    if (tab[i + (nodeTab1[i].getValue() - nodeTab1[i].children[y])][j] > maximalAgreement)
                        maximalAgreement = tab[i + (nodeTab1[i].getValue() - nodeTab1[i].children[y])][j];
                }
            }

            tab[i][j] = maximalAgreement;
            maximalAgreement = 0;
        }
        delete[](tempTab);
    }


    return tab;
}

int main()
{
    int n;
    cin >> n;
    Tree** tree = new Tree * [n];
    NodeProperties** nodeTab = new NodeProperties * [n];
    string newick;
    string tempString = "";
    int temp = 0;
    int maximalAgreement = 0;
    int output;

    for (int i = 0; i < n; i++)
    {
        Tree* newTree = new Tree();
        tree[i] = newTree;
        cin >> newick;

        for (int j = 0; j < int(newick.length()); j++)
        {
            if (newick[j] == '(')
            {
                if (newick[j + 1] >= 48 && newick[j + 1] <= 57)
                {
                    temp = int(j + 1);
                    while (newick[temp] >= 48 && newick[temp] <= 57)
                    {
                        tempString += newick[temp];
                        temp++;
                    }
                    temp = int(toInt(tempString));
                    tree[i]->addChild(temp);
                }
                else
                {
                    tree[i]->addChild(tree[i]->key);
                    tree[i]->key--;
                }
            }
            else if (newick[j] == ',')
            {
                if (newick[j + 1] >= 48 && newick[j + 1] <= 57)
                {
                    temp = int(j + 1);
                    while (newick[temp] >= 48 && newick[temp] <= 57)
                    {
                        tempString += newick[temp];
                        temp++;
                    }
                    temp = int(toInt(tempString));
                    tree[i]->addBrother(temp);
                }
                else
                {
                    tree[i]->addBrother(tree[i]->key);
                    tree[i]->key--;
                }
            }
            else if (newick[j] == ')')
            {
                tree[i]->backToParent();
            }
            else if (newick[j] == ';')
                break;

            tempString = "";
        }
        nodeTab[i] = createNodeTab(tree[i]);
    }

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            int** tab = compareTrees(tree[i], tree[j], nodeTab[i], nodeTab[j]);
            maximalAgreement = tab[tree[i]->getleavesAmount()][tree[i]->getleavesAmount()];
            output = tree[i]->getleavesAmount() - maximalAgreement;
            cout << output << endl;
        }
    }
    delete[](tree);
    delete[](nodeTab);

    return 0;
}