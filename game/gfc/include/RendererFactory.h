/****************************************************************************
GFC - Games Fundamental Classes version 2.70
Copyright (C) 2009-2022 Jarek Francik, Kingston University London

This work is licensed under a Creative Commons Attribution-ShareAlike 4.0 International License:
http://creativecommons.org/licenses/by-sa/4.0/

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Created by Jarek Francik
jarek@kingston.ac.uk
****************************************************************************/
#ifndef __RENDERER_FACTORY_H__
#define __RENDERER_FACTORY_H__

#include "Renderer.h"

#include <iostream>
#include <memory>
#include <unordered_map>
#include <functional>

// Any change to this value require a rebuild of the GFC.DLL module
#define DEFAULT_RENDERER "OGL"

// CRendererSDL class
class EXT_DECL CRendererFactory 
{
public:
    // Function constructing a renderer of the given class - registered by the renderer module
    // should call the simplest possible constructor
    using RendererConstructor = std::function<std::shared_ptr<IRenderer> ()>;

    // procedure to initialises a renderer once constructed - called by the client typicallt using one of the Create function
    // The factory will decide wheter to execute immediately or in a delayed mode 
    // (the latter will be applied for early creation of static objects)
    using RendererCreator = std::function<void (std::shared_ptr<IRenderer>)>;

    static CRendererFactory& Instance()
    {
        static CRendererFactory factory;
        return factory;
    }

    static void Register(const std::string& type, RendererConstructor creator)
    {
        Instance().m_constructors[type] = creator;
    }

    static void SetDefault(const std::string& type)
    {
        auto it = Instance().m_constructors.find(type);
        if (it != Instance().m_constructors.end())
            Instance().m_pDefault = it->second;
    }

    //IRenderer* Create() {
    //    if (constructors.size() == 0)
    //        RegisterRenderers();

    //    if (m_pDefault)
    //        return m_pDefault();
    //    else
    //        return nullptr;
    //}

    static std::shared_ptr<IRenderer> Create(RendererCreator proc, bool bInitialise = false)
    {
        if (Instance().m_constructors.size() == 0)
            RegisterRenderers();
        
        if (!Instance().m_pDefault)
            return nullptr;

        std::shared_ptr<IRenderer> pRenderer = Instance().m_pDefault();

        if (Instance().m_bInitialised)
            proc(pRenderer);
        else
            if (bInitialise)
                Instance().m_creators.push_front(std::pair<RendererCreator, std::shared_ptr<IRenderer> >(proc, pRenderer));
            else
                Instance().m_creators.push_back(std::pair<RendererCreator, std::shared_ptr<IRenderer> >(proc, pRenderer));

        if (bInitialise)
        {
            for (auto pair : Instance().m_creators)
                pair.first(pair.second);

            Instance().m_bInitialised = true;
        }
            
        return pRenderer;
    }

    static void RegisterRenderers();

    //IRenderer* Create(const std::string& type) {
    //    if (constructors.size() == 0)
    //        RegisterRenderers();

    //    auto it = constructors.find(type);
    //    if (it != constructors.end())
    //        return it->second();
    //    else
    //        return nullptr;
    //}

private:
    CRendererFactory() { }

    bool m_bInitialised = false;

#pragma warning(push)
#pragma warning(disable: 4251)
    std::unordered_map<std::string, RendererConstructor> m_constructors;
    std::list<std::pair<RendererCreator, std::shared_ptr<IRenderer> > > m_creators;
    RendererConstructor m_pDefault = NULL;
#pragma warning(pop)

};


#endif
