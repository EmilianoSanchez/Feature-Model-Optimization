#ifndef CONTAINERS_H_INCLUDED
#define CONTAINERS_H_INCLUDED

#include <queue>
#include <stack>

/*! \class Node
 * \brief This class represents the nodes of the state-space graph (solution space) in the best first search algorithm.
 *        It contains a configuration and its heuristic value. The less and greater operators are used
 *        by CPriorityQueue and CPriorityStack objects to order the states according to the heuristic value.
 */
class Node{
public:
    Node(){};
    Node(const Configuration &_conf,double _value=0.0):conf(_conf),value(_value){}
    //! greater operator
    bool operator>(const Node &other) const {
        return value>=other.value;
    };
    //! less operator
    bool operator<(const Node &other) const {
        return value<=other.value;
    };

    Configuration conf;
    double        value;
};


//! class that represents the Container template concept
/*class Container{
public:
    virtual bool empty() const =0;
    virtual void push(const Configuration &conf) =0;
    virtual void pop() =0;
    virtual const Configuration &top() const =0;
};*/

//! Wrapper used to unified the queue interface with the rest of the containers
template<typename Element>
class Queue{
private:
    std::queue<Element> q;
public:
    bool empty() const{
        return q.empty();
    };
    void push(const Element &elem){
        q.push(elem);
    };
    void pop(){
        q.pop();
    }
    const Element &top() const{
        return q.front();
    }
};


//! Class used to implement a priority stack
template<typename Element>
class Priority_Stack{
private:
    std::stack<Element> s;
    std::priority_queue<Element> pq;
public:
    bool empty(){
        while(!pq.empty()){
            s.push(pq.top());
            pq.pop();
        }
        return s.empty();
    };
    void push(const Element &elem){
        pq.push(elem);
    };
    void pop(){
        s.pop();
    }
    const Element &top() const{
        return s.top();
    }
};

//! Used to implement the Depth-first Search strategy
typedef std::stack<Node> CStack;

//! Used to implement the Breadth-first Search strategy
typedef Queue<Node> CQueue;

//! Used to implement the Greedy Best-First Search strategy (GBFS)
typedef Priority_Stack<Node> CPriorityStack;

//! Used to implement the Best-First Search Star strategy (BFS*)
typedef std::priority_queue<Node,std::vector<Node>,std::greater<Node> > CPriorityQueue;




#endif // CONTAINERS_H_INCLUDED
