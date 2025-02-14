/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/std/containers/unordered_set.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/functional.h>
#include <AzCore/std/string/string.h>

namespace AtomToolsFramework
{
    class AtomToolsDocumentRequests;

    //! DocumentTypeInfo is used to provide details about a specific document type and register it with the document system. It defines
    //! the associated document type name, a factory for creating instances of that document type, and contains filters for determining
    //! compatibility between this document type and other file types during different operations. Each document class is responsible for
    //! implementing functions that construct its document type info, which will be registered and used by the document system to guide and
    //! validate different operations.
    struct DocumentTypeInfo final
    {
        //! Function type used for instantiating a document described by this type.
        using FactoryCallback = AZStd::function<AtomToolsDocumentRequests*(const AZ::Crc32&)>;

        //! A pair of strings representing a file type description and extension.
        using ExtensionInfo = AZStd::pair<AZStd::string, AZStd::string>;

        //! Container of registered file types used for an action.
        using ExtensionInfoVector = AZStd::vector<ExtensionInfo>;

        //! Invokes the factory callback to create an instance of a document class.
        AtomToolsDocumentRequests* CreateDocument(const AZ::Crc32& toolId) const;

        //! Helper functions use determine if a file path or extension is supported for an operation.
        bool IsSupportedExtensionToCreate(const AZStd::string& path) const;
        bool IsSupportedExtensionToOpen(const AZStd::string& path) const;
        bool IsSupportedExtensionToSave(const AZStd::string& path) const;
        bool IsSupportedExtension(const ExtensionInfoVector& supportedExtensions, const AZStd::string& path) const;

        //! Retrieves the first registered, or default, extension for saving this document type.
        AZStd::string GetDefaultExtensionToSave() const;

        //! A string used for displaying and searching for this document type.
        AZStd::string m_documentTypeName;

        //! Factory function for creating an instance of the associated document.
        FactoryCallback m_documentFactoryCallback;

        //! Containers for extensions supported by each of the common operations.
        ExtensionInfoVector m_supportedExtensionsToCreate;
        ExtensionInfoVector m_supportedExtensionsToOpen;
        ExtensionInfoVector m_supportedExtensionsToSave;

        //! Used to make the initial selection in the create document dialog.
        AZ::Data::AssetId m_defaultAssetIdToCreate;

        //! Optional asset type used to filter suitable assets for creation
        AZStd::unordered_set<AZ::Uuid> m_supportedAssetTypesToCreate;
    };

    using DocumentTypeInfoVector = AZStd::vector<DocumentTypeInfo>;
} // namespace AtomToolsFramework
