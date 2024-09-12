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
#include "pch.h"
#include "RendererFactory.h"
#include "RendererSDL.h"
#include "RendererOGL.h"

void CRendererFactory::RegisterRenderers()
{
    CRendererFactory::Register("SDL", [] {
        return std::make_shared<CRendererSDL>();
        });
    CRendererFactory::Register("OGL", [] {
        return std::make_shared<CRendererOGL>();
        });

    CRendererFactory::SetDefault(DEFAULT_RENDERER);
}
