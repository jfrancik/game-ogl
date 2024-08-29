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

void RegisterRenderers();

// CRendererSDL class
class EXT_DECL CRendererFactory 
{
public:
    using RendererCreator = std::function<IRenderer* ()>;

    static CRendererFactory& Instance()
    {
        static CRendererFactory factory;
        return factory;
    }

    void Register(const std::string& type, RendererCreator creator)
    {
        creators[type] = creator;
    }

    void SetDefault(const std::string& type)
    {
        auto it = creators.find(type);
        if (it != creators.end())
            m_pDefault = it->second;
    }

    IRenderer* Create() {
        if (creators.size() == 0)
            RegisterRenderers();

        if (m_pDefault)
            return m_pDefault();
        else
            return nullptr;
    }

    IRenderer* Create(const std::string& type) {
        if (creators.size() == 0)
            RegisterRenderers();

        auto it = creators.find(type);
        if (it != creators.end())
            return it->second();
        else
            return nullptr;
    }

private:
    CRendererFactory() { }

#pragma warning(push)
#pragma warning(disable: 4251)
    std::unordered_map<std::string, RendererCreator> creators;
    RendererCreator m_pDefault = NULL;
#pragma warning(pop)

};


#endif
