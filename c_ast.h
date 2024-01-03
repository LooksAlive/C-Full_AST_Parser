#include <stdio.h>
#include <stdlib.h>

// Define NodeType enumeration
enum NodeType {
    NODE_PROGRAM,
    NODE_VARIABLE,
    NODE_CONSTANT,
    // Add more node types as needed
};

// Define Position struct
struct Position {
    int line;
    int column;
};

// Define generic Node struct
struct Node {
    enum NodeType type;
    struct Position position;
};

// Define specific AST nodes
struct VariableNode {
    struct Node base;  // Inherit from generic Node
    const char* name;  // Additional attribute for variable name
};

struct ConstantNode {
    struct Node base;  // Inherit from generic Node
    int value;         // Additional attribute for constant value
};

// Function to create a new Position
struct Position createPosition(int line, int column) {
    struct Position pos;
    pos.line = line;
    pos.column = column;
    return pos;
}

// Function to create a new Node
struct Node createNode(enum NodeType type, struct Position position) {
    struct Node node;
    node.type = type;
    node.position = position;
    return node;
}

// Function to create a new VariableNode
struct VariableNode* createVariableNode(const char* name, struct Position position) {
    struct VariableNode* varNode = (struct VariableNode*)malloc(sizeof(struct VariableNode));
    if (varNode != NULL) {
        varNode->base.type = NODE_VARIABLE;
        varNode->base.position = position;
        varNode->name = name;
    }
    return varNode;
}

// Function to create a new ConstantNode
struct ConstantNode* createConstantNode(int value, struct Position position) {
    struct ConstantNode* constNode = (struct ConstantNode*)malloc(sizeof(struct ConstantNode));
    if (constNode != NULL) {
        constNode->base.type = NODE_CONSTANT;
        constNode->base.position = position;
        constNode->value = value;
    }
    return constNode;
}

// Function to show information about a Node
void showNode(struct Node* node) {
    printf("Type: %d, Line: %d, Column: %d\n", node->type, node->position.line, node->position.column);
}

// Function to show information about a VariableNode
void showVariableNode(struct VariableNode* varNode) {
    showNode(&varNode->base);
    printf("Variable Name: %s\n", varNode->name);
}

// Function to show information about a ConstantNode
void showConstantNode(struct ConstantNode* constNode) {
    showNode(&constNode->base);
    printf("Constant Value: %d\n", constNode->value);
}

// Function to free memory allocated for a Node
void freeNode(struct Node* node) {
    // No dynamic memory to free in the generic Node
    (void)node;
}

// Function to free memory allocated for a VariableNode
void freeVariableNode(struct VariableNode* varNode) {
    freeNode(&varNode->base);
    free(varNode);
}

// Function to free memory allocated for a ConstantNode
void freeConstantNode(struct ConstantNode* constNode) {
    freeNode(&constNode->base);
    free(constNode);
}

