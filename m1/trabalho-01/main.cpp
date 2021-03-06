#include <iostream>
using namespace std;

#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include <queue>

typedef vector<vector<int>> AdjacencyMatrix;

void clear_screen()
{
#ifdef WINDOWS
    std::system("cls");
#else
    // POSIX
    std::system ("clear");
#endif
}

string get_graph_type()
{
    char user_answer;

    cout << "O grafo é direcionado (y/n)? ";
    cin >> user_answer;

    user_answer = tolower(user_answer);

    return user_answer == 'y' ? "directed" : "non-directed";
}

int get_vertices_count()
{
    int vertices;

    cout << "O grafo tem quantos vertices? ";
    cin >> vertices;

    return vertices;
}

void clear_matrix(int** matrix, int rows)
{
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < rows; j++)
            matrix[i][j] = 0;
}

bool is_adjacent_vertices(AdjacencyMatrix matrix, int i, int j)
{
    return matrix[i][j] == 1;
}

bool is_matrix_empty(int** matrix, int rows)
{
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < rows; j++)
            if(matrix[i][j] == 1) return false;

    return true;
}

void print_adjacent_matrix_row(AdjacencyMatrix matrix, int i)
{
    printf("[ %d ] => ", i + 1);
    for(int j = 0; j < matrix.size(); j++)
    {
        if(is_adjacent_vertices(matrix, i, j))
            printf("%d\t", j + 1);
    }
}

void print_adjacent_matrix(AdjacencyMatrix matrix)
{
    for(int i = 0; i < matrix.size(); i++)
    {
        print_adjacent_matrix_row(matrix, i);
        printf("\n");
    }
}

void print_main_menu()
{
    cout << "Escolha uma opção:";
    cout << "\n[ a ] Incluir vertice";
    cout << "\n[ b ] Incluir aresta/arco";
    cout << "\n[ c ] Remover vertice";
    cout << "\n[ d ] Remover aresta/arco";

    cout << "\n\n[ l ] Realizar busca por largura";
    cout << "\n[ p ] Realizar busca por profundidade";

    cout << "\n\n[ q ] Sair";
}

bool validate_selected_menu_option(char option)
{
    return
        option == 'a'
        || option == 'b'
        || option == 'c'
        || option == 'd'

        || option == 'l'
        || option == 'p'

        || option == 'q';
}

char get_valid_menu_option()
{
    char selected_option;
    bool is_valid_option;

    do
    {
        cout << "\n~ ";
        cin >> selected_option;

        selected_option = tolower(selected_option);

        is_valid_option = validate_selected_menu_option(selected_option);
        if(!is_valid_option) cout << "[!] Opcao invalida\n\n";
    }
    while (!is_valid_option);

    return selected_option;
}

void set_graph_vertice(AdjacencyMatrix &matrix, int vertice, vector<int> adjacency_list)
{
    for(int i = 0; i < adjacency_list.size(); i++)
        matrix[vertice][adjacency_list[i] - 1] = 1;

}

vector<int> get_vertice_adjacency_list(int vertice, int vertices_count)
{
    string adjacent_vertices;
    string adjacent_vertice_as_string;
    int adjacent_vertice;

    vector<int> adjacency_list;

    // BUG: the adjacency list needs to begin with a whitespace
    printf("Digite os vertices adjacentes ao vertice %d, separados por espacos (ex: 2 5 9)\n", vertice + 1);
    printf("~ ");

    std::cin.get();
    std::getline(std::cin, adjacent_vertices);

    // adjacent_vertices is a string with the pattern "1 2 3", where each number represents one adjacent vertice of i
    std::istringstream iss(adjacent_vertices);

    while(iss >> adjacent_vertice_as_string)
    {
        adjacent_vertice = stoi(adjacent_vertice_as_string);

        if (adjacent_vertice > vertices_count) continue;

        adjacency_list.push_back(adjacent_vertice);
    }

    return adjacency_list;
}

void get_graph_initial_state(AdjacencyMatrix &matrix)
{
    string adjacent_vertices;
    string vertice;

    vector<int> vertice_adjacency_list;

    for(int i = 0; i < matrix.size(); i++)
    {
        vertice_adjacency_list = get_vertice_adjacency_list(i, matrix.size());

        cout << endl;

        set_graph_vertice(matrix, i, vertice_adjacency_list);
    }
}

void add_graph_vertice(AdjacencyMatrix &matrix)
{
    int vertices_count = matrix.size();
    vector<int> empty_vertice(vertices_count + 1, 0);
    vector<int> vertice_adjacency_list;

    // include the new vertice in the existent vertices adjacency list
    for(int i = 0; i < vertices_count; i++)
    {
        matrix[i].push_back(0);
    }

    matrix.push_back(empty_vertice);

    vertice_adjacency_list = get_vertice_adjacency_list(vertices_count, vertices_count + 1);
    set_graph_vertice(matrix, vertices_count, vertice_adjacency_list);
}

int get_selected_vertice(string message)
{
    int selected_vertice;
    cout << message << endl;
    cout << "~ ";
    cin >> selected_vertice;

    return selected_vertice - 1;
}

void get_new_adjacency(AdjacencyMatrix &matrix)
{
    int vertice = get_selected_vertice("Em qual vertice voce deseja incluir uma aresta/arco?");
    int new_adjacency = get_selected_vertice("Qual vertice sera o novo vertice adjacente?");

    matrix[vertice][new_adjacency] = 1;
}

void remove_graph_vertice(AdjacencyMatrix &matrix, int vertice_to_remove)
{
    if (vertice_to_remove >= matrix.size()) return;

    matrix.erase(matrix.begin() + vertice_to_remove);

    for(int i = 0; i != matrix.size(); i++)
    {
        matrix[i].erase(matrix[i].begin() + vertice_to_remove);
    }
}

void remove_vertice_adjacency(AdjacencyMatrix &matrix)
{
    int vertice = get_selected_vertice("Voce deseja remover uma aresta/arco de qual vertice?");
    int adjacency = get_selected_vertice("Com qual vertice?");

    if (vertice >= matrix.size() || adjacency >= matrix.size()) return;

    matrix[vertice][adjacency] = 0;
}

bool has_unvisited_vertices(vector<bool> vertices)
{
    for(bool visited_vertice : vertices)
        if (!visited_vertice) return true;

    return false;
}

int get_next_unvisited_adjacent_vertice(vector<int> vertice, vector<bool> visited_vertices)
{
    for(int i = 0; i < vertice.size(); i++)
        if(vertice[i] == 1 && !visited_vertices[i]) return i;

    return -1;
}

int get_next_unvisited_vertice(vector<bool> visited_vertices)
{
    for(int i = 0; i < visited_vertices.size(); i++)
        if(!visited_vertices[i]) return i;

    return -1;
}

vector<int> graph_depth_first_search(AdjacencyMatrix matrix, char search_type)
{
    vector<bool> visited_vertices(matrix.size(), false);
    vector<int> visiting_order;
    stack<int> vertices_stack;
    int searched_vertice = -1;

    if(search_type == 'v')
        searched_vertice = get_selected_vertice("Qual vertice voce procura?");

    int root = get_selected_vertice("Qual deve ser o vertice root da busca?");

    while(has_unvisited_vertices(visited_vertices))
    {
        // has no more adjacent vertices
        if (root == -1)
        {
            vertices_stack.pop();
            root = vertices_stack.empty() ? get_next_unvisited_vertice(visited_vertices) : vertices_stack.top();
            root = get_next_unvisited_adjacent_vertice(matrix[root], visited_vertices);
            continue;
        }

        visited_vertices[root] = true;
        visiting_order.push_back(root);
        vertices_stack.push(root);

        if(root == searched_vertice)
        {
            return visiting_order;
        }

        root = get_next_unvisited_adjacent_vertice(matrix[root], visited_vertices);
    }

    cout << "\n\n nao achei essa desgraça \n\n";

    return visiting_order;
}

vector<int> graph_depth_breadth_search(AdjacencyMatrix matrix, char search_type)
{
    vector<bool> visited_vertices(matrix.size(), false);
    vector<int> visiting_order;
    queue<int> vertices_queue;
    int searched_vertice = -1;
    int next_adjacent;

    if(search_type == 'v')
        searched_vertice = get_selected_vertice("Qual vertice voce procura?");

    int root = get_selected_vertice("Qual deve ser o vertice root da busca?");


    visited_vertices[root] = true;
    visiting_order.push_back(root);

    if (root == searched_vertice) return visiting_order;

    while(has_unvisited_vertices(visited_vertices))
    {
        next_adjacent = get_next_unvisited_adjacent_vertice(matrix[root], visited_vertices);

        while (next_adjacent != -1)
        {
            visiting_order.push_back(next_adjacent);

            if (next_adjacent == searched_vertice) return visiting_order;

            visited_vertices[next_adjacent] = true;
            vertices_queue.push(next_adjacent);
            next_adjacent = get_next_unvisited_adjacent_vertice(matrix[root], visited_vertices);
        }

        root = vertices_queue.empty() ? get_next_unvisited_vertice(visited_vertices) : vertices_queue.front();
        vertices_queue.pop();
    }

    return visiting_order;
}

char get_search_type()
{
    char search_type;
    cout << "Digite \"v\" para buscar um vertice especifico ou \"a\" para ver todos os vertices na ordem de visitacao";
    cout << "\n\~ ";
    cin >> search_type;

    return search_type;
}

int main()
{
    bool is_directed_graph;
    int vertices_count;
    char selected_menu_option;

    is_directed_graph = get_graph_type() == "directed";
    vertices_count = get_vertices_count();

    AdjacencyMatrix adj_mat(vertices_count, vector<int>(vertices_count, 0));

    cout << "\n\n";

    get_graph_initial_state(adj_mat);


    clear_screen();

    do
    {
        print_adjacent_matrix(adj_mat);

        cout << "\n\n";

        print_main_menu();
        selected_menu_option = get_valid_menu_option();

        switch(selected_menu_option)
        {
        case 'a': // add one vertice
            cout << "\n\n";
            vertices_count++;
            add_graph_vertice(adj_mat);
            clear_screen();
            break;

        case 'b': // add new edge/arc
            cout << "\n\n";
            get_new_adjacency(adj_mat);
            clear_screen();
            break;

        case 'c': // remove vertice
            cout << "\n\n";
            remove_graph_vertice(adj_mat, get_selected_vertice("Qual vertice voce deseja remover?"));
            clear_screen();
            break;

        case 'd': // remove edge/arc
            cout << "\n\n";
            remove_vertice_adjacency(adj_mat);
            clear_screen();
            break;
        case 'p': // search by depth
        {
            cout << "\n\n";
            vector<int> visiting_order;
            visiting_order = graph_depth_first_search(adj_mat, get_search_type());
            cout << "Ordem de visitacao -> [\t";
            for(int i = 0; i < visiting_order.size(); i++)
            {
                printf("%d\t", visiting_order[i] + 1);
            }

            cout << "\t]\n\n";
            break;
        }

        case 'l': // search by breadth
        {
            cout << "\n\n";
            vector<int> visiting_order;
            visiting_order = graph_depth_breadth_search(adj_mat, get_search_type());
            cout << "Ordem de visitacao -> [\t";
            for(int i = 0; i < visiting_order.size(); i++)
            {
                printf("%d\t", visiting_order[i] + 1);
            }

            cout << "\t]\n\n";
            break;
        }

        case 'q':

            break;

        default:
            cout << "\n\n\n" << selected_menu_option << "\n\n\n";
            printf("\n[!] Opcao invalida\n\n");
            continue;
        }
    }
    while (selected_menu_option != 'q');

    cout << "\to/";

    return 0;
}
