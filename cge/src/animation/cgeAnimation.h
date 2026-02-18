/*
* cgeAnimation.h
*
*  Created on: 2014-9-9
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#if !defined(_CGEANIMATION_H_) && !defined(_CGE_ONLY_FILTERS_)
#define _CGEANIMATION_H_
#include "cgeAction.h"

namespace CGE
{
	template<class ActionType>
	class AnimationInterfaceAbstract
	{
	public:
		AnimationInterfaceAbstract() : m_startTime(0), m_endTime(0), m_shouldRender(true) {}
		AnimationInterfaceAbstract(float start, float end) : m_startTime(start), m_endTime(end), m_shouldRender(true) {}

		virtual ~AnimationInterfaceAbstract()
		{
			clearActions(true);
		}

		typedef ActionType AnimationActionType;

		template<class RealActionType>
		void pushAction(RealActionType* action)
		{
			typename RealActionType::AnimType* anim = dynamic_cast<typename RealActionType::AnimType*>(this);

			// If you hit this assertion, check: the Action template parameter type must match the type of the pushed sprite
			assert(anim != nullptr);

			action->bind(anim);
			m_timeActions.push_back(action);
		}

		virtual void clearActions(bool bDelete = true)
		{
			if(bDelete)
			{
				for(typename std::vector<ActionType*>::iterator iter = m_timeActions.begin();
					iter != m_timeActions.end(); ++iter)
				{
					delete *iter;
				}
			}
			m_timeActions.clear();
			m_actions2Run.clear();
		}

		virtual bool run(float currentTime)
		{
			if(m_actions2Run.empty())
				return false;
			float t = currentTime - m_startTime;

			for(typename std::vector<ActionType*>::iterator iter = m_actions2Run.begin();
				iter != m_actions2Run.end(); )
			{
				ActionType* action = *iter;
				if(t >= action->endTime())
				{
					action->actionStop();
					iter = m_actions2Run.erase(iter);
					continue;
				}
				if(t >= action->startTime())
				{
					action->act((t - action->startTime()) / action->duringTime());
				}
				++iter;
			}
			return true;
		}

		bool runTo(float time) // Jump to progress; handled directly by the timeline
		{
			return false;
		}

		virtual void animationStart() // Animation start function; resets all actions to their initial state.
		{
			showSprite(true);
			for(typename std::vector<ActionType*>::iterator iter = m_timeActions.begin(); iter != m_timeActions.end(); ++iter)
			{
				(*iter)->actionStart();
			}
			m_actions2Run = m_timeActions;
		}

		virtual void animationEnd() // Animation end function; ensures the animation is in its final state after the time period ends.
		{
			for(typename std::vector<ActionType*>::iterator iter = m_actions2Run.begin(); iter != m_actions2Run.end(); ++iter)
			{
				(*iter)->actionStop();
			}
			m_actions2Run.clear();
		}

		virtual void render() = 0; // Must be implemented (if not needed, implement as an empty function).

		// Must be implemented by subclass; otherwise it should not be called (if not needed, implement as an empty function).
		virtual void _renderWithFather(AnimationInterfaceAbstract*)
		{
			CGE_LOG_ERROR("Logic should not be added as child sprite!\n");
		}

		inline float& startTime() { return m_startTime; }
		inline float& endTime() { return m_endTime; }
		inline std::vector<ActionType*>& timeActions() { return m_timeActions; }
		inline std::vector<ActionType*>& actions2Run() { return m_actions2Run; }
		inline void showSprite(bool bShow) { m_shouldRender = bShow; }
		

		static bool compSpriteZ(const AnimationInterfaceAbstract* left, const AnimationInterfaceAbstract* right)
		{
			return left->_getZ() < right->_getZ();
		}

		inline bool operator<(const AnimationInterfaceAbstract<ActionType>& other) const
		{
			return m_startTime < other.m_startTime;
		}

		static inline bool compTp(const AnimationInterfaceAbstract<ActionType>* left, const AnimationInterfaceAbstract<ActionType>* right)
		{
			return left->m_startTime < right->m_startTime;
		}

		static inline bool compT(const AnimationInterfaceAbstract<ActionType>& left, const AnimationInterfaceAbstract<ActionType>& right)
		{
			return left.m_startTime < right.m_startTime;
		}

	protected:
		virtual float _getZ() const { return -1e20f; };


	protected:
		float m_startTime;
		float m_endTime;
		bool m_shouldRender;
		std::vector<ActionType*> m_timeActions; // Array of actions that will complete their assigned motion within the scheduled time
		std::vector<ActionType*> m_actions2Run; // Actions that have not yet completed after the timeline started.

	};

	template<class ActionType>
	class AnimationWithChildrenInterface : public AnimationInterfaceAbstract<ActionType>
	{
	public:
		typedef AnimationInterfaceAbstract<ActionType> AnimationInterfaceType;

		AnimationWithChildrenInterface() : AnimationInterfaceType() {}
		AnimationWithChildrenInterface(float start, float end) : AnimationInterfaceType(start, end) {}
		virtual ~AnimationWithChildrenInterface()
		{
			clearChildren(true);
		}

		inline std::vector<AnimationInterfaceType*>& children() { return m_childAnimation; }

		virtual void clearChildren(bool bDelete = true)
		{
			if(bDelete)
			{
				for(typename std::vector<AnimationInterfaceType*>::iterator iter = m_childAnimation.begin(); iter != m_childAnimation.end(); ++iter)
				{
					delete *iter;
				}
			}
			m_childAnimation.clear();
			m_children2Run.clear();
		}

		inline void pushChild(AnimationInterfaceType* child) { assert(child != this && child != nullptr); m_childAnimation.push_back(child); }		

		virtual bool run(float currentTime)
		{
			AnimationInterfaceType::run(currentTime);
			for(typename std::vector<AnimationInterfaceType*>::iterator iter = m_children2Run.begin(); iter != m_children2Run.end();)
			{
				AnimationInterfaceType* anim = *iter;
				if(anim->startTime() > currentTime)
					anim->showSprite(false);
				else
					anim->showSprite(true);
				if(!anim->run(currentTime))
				{
					iter = m_children2Run.erase(iter);
				}
				else ++iter;
			}
			return currentTime < this->m_endTime;
		}

		virtual void animationStart()
		{
			AnimationInterfaceType::animationStart();
			for(typename std::vector<AnimationInterfaceType*>::iterator iter = m_childAnimation.begin(); iter != m_childAnimation.end(); ++iter)
			{
				(*iter)->animationStart();
			}
			m_children2Run = m_childAnimation;
		}

		virtual void animationEnd()
		{
			AnimationInterfaceType::animationEnd();
			for(typename std::vector<AnimationInterfaceType*>::iterator iter = m_children2Run.begin(); iter != m_children2Run.end(); ++iter)
			{
				(*iter)->animationEnd();
			}
			m_children2Run.clear();
		}

	protected:
		std::vector<AnimationInterfaceType*> m_childAnimation;
		std::vector<AnimationInterfaceType*> m_children2Run;

	};

	/////////////////// Timeline section ////////////////////////


	template<class AnimationType>
	class TimeLineInterface : private AnimationInterfaceAbstract<typename AnimationType::AnimationActionType>
	{
	public:
		TimeLineInterface(float totalTime = 100.0f) : AnimationInterfaceAbstract<typename AnimationType::AnimationActionType>(), m_currentTime(0), m_totalTime(totalTime), m_isStarted(false){}
        ~TimeLineInterface() { CGE_LOG_INFO("Time line release...\n"); clear(); }

		typedef AnimationInterfaceAbstract<typename AnimationType::AnimationActionType> FatherAnimationType;

		using FatherAnimationType::pushAction;
		using FatherAnimationType::clearActions;
		using FatherAnimationType::timeActions;
		using FatherAnimationType::actions2Run;

		void setTotalTime(float totalTime)
		{
			m_totalTime = totalTime;
		}

		void push(AnimationType* obj)
		{
			m_timeObjects.push_back(obj);
		}

		void clear(bool bDelete = true, bool bResetActions = true)
		{
			if(bDelete)
			{
				for(typename std::vector<AnimationType*>::iterator iter = m_timeObjects.begin(); iter != m_timeObjects.end(); ++iter)
				{
					delete *iter;
				}
			}
			m_timeObjects.clear();
			m_objectsWait2Render.clear();
			m_objects2Render.clear();

			if(bResetActions)
				FatherAnimationType::clearActions(true);
		}

		// startTime is optional, defaults to 0
		void start(float startTime = 0.0f)
		{
			m_isStarted = true;
			m_currentTime = startTime;
			std::stable_sort(m_timeObjects.begin(), m_timeObjects.end(), AnimationType::compTp);

			this->animationStart();

			for(typename std::vector<AnimationType*>::iterator iter = m_timeObjects.begin(); iter != m_timeObjects.end(); ++iter)
			{
				(*iter)->animationStart();
			}
			m_objectsWait2Render = m_timeObjects;
		}

		// Set the entire scene to its end state
		void end()
		{
			m_isStarted = false;
			m_objects2Render.clear();
			m_objectsWait2Render.clear();
		}

		// Update based on elapsed time. Ensure time > 0.
		// Call start() before update() to ensure the scene is initialized.
		bool update(float deltaTime)
		{
			if(!m_isStarted || m_objectsWait2Render.empty())
				return false;
			m_objects2Render.clear();
			m_currentTime += deltaTime;
			if(m_currentTime > m_totalTime)
				return false;

			this->run(m_currentTime);

			for(typename std::vector<AnimationType*>::iterator iter = m_objectsWait2Render.begin(); iter != m_objectsWait2Render.end();)
			{
				AnimationType* anim = *iter;

				if(m_currentTime >= anim->endTime())
				{
					anim->animationEnd();
					m_objects2Render.push_back(anim);
					iter = m_objectsWait2Render.erase(iter);
					continue;
				}
				else if(m_currentTime >= anim->startTime())
				{
					anim->run(m_currentTime);
					m_objects2Render.push_back(anim);
				}
				else break;
				++iter;
			}

			return true;
		}

		void updateTo(float currentTime) // Jump to progress; performs interpolation across the entire timeline, which may be slow
		{
			m_currentTime = currentTime;

			this->animationStart();

			std::stable_sort(m_timeObjects.begin(), m_timeObjects.end(), AnimationType::compTp);

			for(typename std::vector<AnimationType*>::iterator iter = m_timeObjects.begin(); iter != m_timeObjects.end(); ++iter)
			{
				(*iter)->animationStart();
			}
			m_objectsWait2Render = m_timeObjects;

			m_objects2Render.clear();

			this->run(m_currentTime);

			for(typename std::vector<AnimationType*>::iterator iter = m_objectsWait2Render.begin(); iter != m_objectsWait2Render.end();)
			{
				AnimationType* anim = *iter;

				if(m_currentTime >= anim->endTime())
				{
					anim->animationEnd();
					iter = m_objectsWait2Render.erase(iter);
					continue;
				}
				else if(m_currentTime >= anim->startTime())
				{
					anim->run(m_currentTime);
					m_objects2Render.push_back(anim);
				}
				else break;
				++iter;
			}
		}

		void render()
		{
			std::stable_sort(m_objects2Render.begin(), m_objects2Render.end(), AnimationType::compSpriteZ);

			for(typename std::vector<AnimationType*>::iterator iter = m_objects2Render.begin();
				iter != m_objects2Render.end(); ++iter)
			{
				(*iter)->render();
			}
		}

		inline float getProgressRate() { return m_currentTime / m_totalTime; }
        inline float totalTime() { return m_totalTime; }
		inline float currentTime() { return m_currentTime; }
		inline void setCurrentTime(float t) { m_currentTime = t; }
		inline std::vector<AnimationType*>& timeObjects() { return m_timeObjects; }
		inline std::vector<AnimationType*>& objects2Render() { return m_objects2Render; }
		inline bool isStarted() { return m_isStarted; }

	protected:
		float m_currentTime;
		float m_totalTime;
		std::vector<AnimationType*> m_timeObjects;

		// All timeObjects waiting to be rendered after animation starts (those that have finished rendering will be removed from the queue)
		std::vector<AnimationType*> m_objectsWait2Render;
		// timeObjects to be rendered each frame (continuously updated); sorted by z-value and filtered to exclude nodes that do not need rendering.
		std::vector<AnimationType*> m_objects2Render;

		bool m_isStarted;
	};

}

#endif