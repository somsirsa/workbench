
/*LICENSE_START*/
/* 
 *  Copyright 1995-2011 Washington University School of Medicine 
 * 
 *  http://brainmap.wustl.edu 
 * 
 *  This file is part of CARET. 
 * 
 *  CARET is free software; you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation; either version 2 of the License, or 
 *  (at your option) any later version. 
 * 
 *  CARET is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details. 
 * 
 *  You should have received a copy of the GNU General Public License 
 *  along with CARET; if not, write to the Free Software 
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 * 
 */ 

#include <algorithm>

#define __SESSION_MANAGER_DECLARE__
#include "SessionManager.h"
#undef __SESSION_MANAGER_DECLARE__

#include "BrowserTabContent.h"
#include "CaretAssert.h"
#include "EventManager.h"
#include "EventBrowserTabDelete.h"
#include "EventBrowserTabNew.h"
#include "EventModelDisplayControllerAdd.h"
#include "EventModelDisplayControllerDelete.h"
#include "EventModelDisplayControllerGetAll.h"

using namespace caret;


/**
 * Constructor.
 */
SessionManager::SessionManager()
: CaretObject(), EventListenerInterface()
{
    for (int32_t i = 0; i < BrainConstants::MAXIMUM_NUMBER_OF_BROWSER_TABS; i++) {
        this->browserTabs[i] = NULL;
    }
    
    EventManager::get()->addEventListener(this, EventTypeEnum::EVENT_BROWSER_TAB_DELETE);
    EventManager::get()->addEventListener(this, EventTypeEnum::EVENT_BROWSER_TAB_NEW);
    EventManager::get()->addEventListener(this, EventTypeEnum::EVENT_MODEL_DISPLAY_CONTROLLER_ADD);
    EventManager::get()->addEventListener(this, EventTypeEnum::EVENT_MODEL_DISPLAY_CONTROLLER_DELETE);
    EventManager::get()->addEventListener(this, EventTypeEnum::EVENT_MODEL_DISPLAY_CONTROLLER_GET_ALL);
}

/**
 * Destructor.
 */
SessionManager::~SessionManager()
{
    EventManager::get()->removeAllEventsFromListener(this);
}

/**
 * Create the session manager.
 * This must be called one AND ONLY one time prior to any
 * other Caret mechanisms.
 */
void 
SessionManager::createSessionManager()
{
    CaretAssertMessage((SessionManager::singletonSessionManager == NULL), 
                       "Session manager has already been created.");

    /*
     * Need to create event manager first.
     */
    EventManager::createEventManager();
    
    SessionManager::singletonSessionManager = new SessionManager();
}

/**
 * Delete the session manager.
 * This may only be called one time after session manager is created.
 */
void 
SessionManager::deleteSessionManager()
{
    CaretAssertMessage((SessionManager::singletonSessionManager != NULL), 
                       "Session manager does not exist, cannot delete it.");
    
    delete SessionManager::singletonSessionManager;
    SessionManager::singletonSessionManager = NULL;
    
    /*
     * Session manager must be deleted before the event
     * manager is deleted.
     */
    EventManager::deleteEventManager();
}

/**
 * Get the one and only session manager.
 *
 * @return  Pointer to the session manager.
 */
SessionManager* 
SessionManager::get()
{
    CaretAssertMessage(SessionManager::singletonSessionManager,
                       "Session manager was not created.\n"
                       "It must be created with SessionManager::createSessionManager().");
    
    return this->singletonSessionManager;
}

/**
 * Get a description of this object's content.
 * @return String describing this object's content.
 */
AString 
SessionManager::toString() const
{
    return "SessionManager";
}

/**
 * Receive events.
 *
 * @param event
 *    Event that needs to be processed.
 */
void 
SessionManager::receiveEvent(Event* event)
{
    if (event->getEventType() == EventTypeEnum::EVENT_BROWSER_TAB_NEW) {
        EventBrowserTabNew* tabEvent =
            dynamic_cast<EventBrowserTabNew*>(event);
        CaretAssert(tabEvent);
        
        for (int32_t i = 0; i < BrainConstants::MAXIMUM_NUMBER_OF_BROWSER_TABS; i++) {
            if (this->browserTabs[i] == NULL) {
                BrowserTabContent* tab = new BrowserTabContent(i);
                this->browserTabs[i] = tab;
                tabEvent->setBrowserTab(tab);
                break;
            }
        }
    }
    else if (event->getEventType() == EventTypeEnum::EVENT_BROWSER_TAB_DELETE) {
        EventBrowserTabDelete* tabEvent =
        dynamic_cast<EventBrowserTabDelete*>(event);
        CaretAssert(tabEvent);
        
        BrowserTabContent* tab = tabEvent->getBrowserTab();
        
        for (int32_t i = 0; i < BrainConstants::MAXIMUM_NUMBER_OF_BROWSER_TABS; i++) {
            if (this->browserTabs[i] == tab) {
                delete this->browserTabs[i];
                this->browserTabs[i] = NULL;
                break;
            }
        }
    }
    else if (event->getEventType() == EventTypeEnum::EVENT_MODEL_DISPLAY_CONTROLLER_ADD) {
        EventModelDisplayControllerAdd* addModelsEvent =
        dynamic_cast<EventModelDisplayControllerAdd*>(event);
        CaretAssert(addModelsEvent);
        
        std::cout << "Received add models event in " << __func__ << std::endl;
        
        this->modelDisplayControllers.push_back(addModelsEvent->getModelDisplayController());
    }
    else if (event->getEventType() == EventTypeEnum::EVENT_MODEL_DISPLAY_CONTROLLER_DELETE) {
        EventModelDisplayControllerDelete* deleteModelsEvent =
        dynamic_cast<EventModelDisplayControllerDelete*>(event);
        CaretAssert(deleteModelsEvent);
        
        std::cout << "Received delete models event in " << __func__ << std::endl;
        
        ModelDisplayController* model = deleteModelsEvent->getModelDisplayController();
        
        std::vector<ModelDisplayController*>::iterator iter =
        std::find(this->modelDisplayControllers.begin(),
                  this->modelDisplayControllers.end(),
                  model);
        
        CaretAssertMessage(iter != this->modelDisplayControllers.end(),
                           "Trying to delete non-existent model controller");
        
        this->modelDisplayControllers.erase(iter);
    }
    else if (event->getEventType() == EventTypeEnum::EVENT_MODEL_DISPLAY_CONTROLLER_GET_ALL) {
        EventModelDisplayControllerGetAll* getModelsEvent =
        dynamic_cast<EventModelDisplayControllerGetAll*>(event);
        CaretAssert(getModelsEvent);
        
        std::cout << "Received get models event in " << __func__ << std::endl;
        
        getModelsEvent->addModelDisplayControllers(this->modelDisplayControllers);
    }
}

