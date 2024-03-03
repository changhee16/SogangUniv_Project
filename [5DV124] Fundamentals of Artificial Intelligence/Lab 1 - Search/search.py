# search.py
# ---------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


"""
In search.py, you will implement generic search algorithms which are called by
Pacman agents (in searchAgents.py).
"""

import util

class SearchProblem:
    """
    This class outlines the structure of a search problem, but doesn't implement
    any of the methods (in object-oriented terminology: an abstract class).

    You do not need to change anything in this class, ever.
    """

    def getStartState(self):
        """
        Returns the start state for the search problem.
        """
        util.raiseNotDefined()

    def isGoalState(self, state):
        """
          state: Search state

        Returns True if and only if the state is a valid goal state.
        """
        util.raiseNotDefined()

    def getSuccessors(self, state):
        """
          state: Search state

        For a given state, this should return a list of triples, (successor,
        action, stepCost), where 'successor' is a successor to the current
        state, 'action' is the action required to get there, and 'stepCost' is
        the incremental cost of expanding to that successor.
        """
        util.raiseNotDefined()

    def getCostOfActions(self, actions):
        """
         actions: A list of actions to take

        This method returns the total cost of a particular sequence of actions.
        The sequence must be composed of legal moves.
        """
        util.raiseNotDefined()


def tinyMazeSearch(problem):
    """
    Returns a sequence of moves that solves tinyMaze.  For any other maze, the
    sequence of moves will be incorrect, so only use this for tinyMaze.
    """
    from game import Directions
    s = Directions.SOUTH
    w = Directions.WEST
    #print("s:", s)
    return  [s, s, w, s, w, w, s, w]
    #return ['South', 'South', 'West']
def depthFirstSearch(problem: SearchProblem):
    """
    Search the deepest nodes in the search tree first.

    Your search algorithm needs to return a list of actions that reaches the
    goal. Make sure to implement a graph search algorithm.

    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:

    print("Start:", problem.getStartState())
    print("Is the start a goal?", problem.isGoalState(problem.getStartState()))
    print("Start's successors:", problem.getSuccessors(problem.getStartState()))
    """
    "*** YOUR CODE HERE ***"
    visited = [] #store visited
    node = util.Stack() #node 자료구조는 stack 
    node.push((problem.getStartState(), []))
    #visited.append(problem.getStartState()) #tmp

    while node.isEmpty() == False:
        cur_loc, dir = node.pop()
        visited.append(cur_loc)
        if (problem.isGoalState(cur_loc)): #check whether it reaches goal
            return dir
        else:
            for successor, action, step in problem.getSuccessors(cur_loc):
                if successor not in visited:
                    node.push((successor, dir + [action]))
                    #visited.append(successor)
    return []

def breadthFirstSearch(problem: SearchProblem):
    """Search the shallowest nodes in the search tree first."""
    "*** YOUR CODE HERE ***"
    #util.raiseNotDefined()
    visited = [] #방문한 기록 저장
    node = util.Queue() #node 자료구조는 queue
    node.push((problem.getStartState(), []))
    visited.append(problem.getStartState())

    while node.isEmpty() == False:
        cur_loc, dir = node.pop()
        #visited.append(cur_loc)
        if (problem.isGoalState(cur_loc)): #목표 지점에 도달 
            return dir
        else:
            
            for successor, action, step in problem.getSuccessors(cur_loc):
                #print("successor: ", successor, "action: ", action)
                if successor not in visited:
                    node.push((successor, dir + [action]))
                    visited.append(successor)
    return []

def uniformCostSearch(problem: SearchProblem):
    """Search the node of least total cost first."""
    "*** YOUR CODE HERE ***"
    visited = util.Counter() #방문한 기록 저장을 Counter로 해보자 
    node = util.PriorityQueue() #node 자료구조는 stack 
    node.push(priority=0, item = (problem.getStartState(), [], 0))
    visited[problem.getStartState()] = 0 #tmp
    while node.isEmpty() == False:
        cur_loc, dir, tmp_pri= node.pop()
        if (problem.isGoalState(cur_loc)): #목표 지점에 도달 
            return dir
        else:
            for successor, action, step in problem.getSuccessors(cur_loc):
                if successor not in visited or visited[successor] > tmp_pri + step:
                    node.push(priority = tmp_pri + step, item = (successor, dir + [action], tmp_pri + step))
                    visited[successor] =  tmp_pri + step
    return []

def nullHeuristic(state, problem=None):
    """
    A heuristic function estimates the cost from the current state to the nearest
    goal in the provided SearchProblem.  This heuristic is trivial.
    """
    
    return 0

def aStarSearch(problem: SearchProblem, heuristic=nullHeuristic):
    """Search the node that has the lowest combined cost and heuristic first."""
    "*** YOUR CODE HERE ***"
    visited = util.Counter() #방문한 기록 저장을 Counter로 해보자 
    node = util.PriorityQueue() #node 자료구조는 stack 
    start_loc = problem.getStartState()
    node.push(priority=0, item = (start_loc, [], 0))
    visited[problem.getStartState()] = 0 
    while node.isEmpty() == False:
        cur_loc, dir, tmp_pri= node.pop()
        if (problem.isGoalState(cur_loc)): #목표 지점에 도달 
            return dir
        else:
            for successor, action, step in problem.getSuccessors(cur_loc):
                new_pri = tmp_pri + step + heuristic(successor, problem) #과거 + 햔제 
                if successor not in visited or visited[successor] > new_pri:
                    node.push(priority = new_pri, item = (successor, dir + [action], tmp_pri + step))
                    visited[successor] =  new_pri
    return []    

# Abbreviations
bfs = breadthFirstSearch
dfs = depthFirstSearch
astar = aStarSearch
ucs = uniformCostSearch
