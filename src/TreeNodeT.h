/*
A TreeNode in the decision tree.
I tried to keep this independent of UCT/MCTS.
Only contains information / methods related to State, Action, Parent, Children etc. 

*/

#pragma once

#include <memory>
#include <math.h>
#include <vector>


namespace msa {
	namespace mcts {

		template <class State, class Action>
		class TreeNodeT {

			typedef std::shared_ptr< TreeNodeT<State, Action> > TreeNodePtr;
			typedef std::shared_ptr< State > StatePtr;
			//			typedef std::shared_ptr< Action > ActionPtr;

		public:
			//--------------------------------------------------------------
			TreeNodeT(const State& state, TreeNodeT* parent = NULL):
				state(state),
				action(),
				parent(parent),
				num_visits(0),
				value(0),
				depth(parent ? parent->depth + 1 : 0)
			{
			}


			//--------------------------------------------------------------
			// expand by adding a single child
			TreeNodeT* expand() {
				// sanity check that we're not already fully expanded
				if(is_fully_expanded()) return NULL;

				// sanity check that we don't have more children than we do actions
				//assert(children.size() < actions.size()) ;

				// if this is the first expansion and we haven't yet got all of the possible actions
				if(actions.empty()) {
					// retrieve list of actions from the state
					state.get_actions(actions);

					// randomize the order
					// QUESTION: should I use a RandomGenerator?
					std::random_shuffle(actions.begin(), actions.end());
				}

				// add the next action in queue as a child
				return add_child_with_action( actions[children.size()] );
			}


			//--------------------------------------------------------------
			void update(float value) {
				this->value += value;
				num_visits++;
			}


			//--------------------------------------------------------------
			// getters
			// state of the TreeNode
			const State& get_state() const { return state; }

			// the action that led to this state
			const Action& get_action() const { return action; }

			// all children have been expanded and simulated
			bool is_fully_expanded() const { return children.empty() == false && children.size() == actions.size(); }

			// does this TreeNode end the search (i.e. the game)
			bool is_terminal() const { return state.is_terminal(); }

			// number of times the TreeNode has been visited
			int get_num_visits() const { return num_visits; }

			// accumulated value (wins)
			float get_value() const { return value; }

			// how deep the TreeNode is in the tree
			int get_depth() const { return depth; }

			// number of children the TreeNode has
			int get_num_children() const { return children.size(); }

			// get the i'th child
			TreeNodeT* get_child(int i) const { return children[i].get(); }

			// get parent
			TreeNodeT* get_parent() const { return parent; }

		private:
			State state;			// the state of this TreeNode
			Action action;			// the action which led to the state of this TreeNode
			TreeNodeT* parent;		// parent of this TreeNode

			int num_visits;			// number of times TreeNode has been visited
			float value;			// value of this TreeNode
			int depth;

			std::vector< TreeNodePtr > children;	// all current children
			std::vector< Action > actions;			// possible actions from this state


			//--------------------------------------------------------------
			// create a clone of the current state, apply action, and add as child
			TreeNodeT* add_child_with_action(const Action& new_action) {
				// create a new TreeNode with the same state (will get cloned) as this TreeNode
				TreeNodeT* child_node = new TreeNodeT(state, this);

				// set the action of the child to be the new action
				child_node->action = new_action;

				// apply the new action to the state of the child TreeNode
				child_node->state.apply_action(new_action);

				// add to children
				children.push_back(TreeNodePtr(child_node));

				return child_node;
			}

		};

		/*
		template <class State, class Action>
		TreeNodeT* add_child_with_action(const Action& new_action) {
		// create a new TreeNode with the same state (will get cloned) as this TreeNode
		TreeNode* child_node = new NodeT(state, this);

		// set the action of the child to be the new action
		child_node->action = new_action;

		// apply the new action to the state of the child TreeNode
		child_node->state.apply_action(new_action);

		// add to children
		children.push_back(TreeNodePtr(child_node));

		return child_node;
		}
		*/
	}
}