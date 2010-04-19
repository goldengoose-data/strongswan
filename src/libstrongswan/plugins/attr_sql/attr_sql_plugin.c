/*
 * Copyright (C) 2008 Martin Willi
 * Hochschule fuer Technik Rapperswil
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#include <library.h>
#include <debug.h>

#include "attr_sql_plugin.h"
#include "sql_attribute.h"

typedef struct private_attr_sql_plugin_t private_attr_sql_plugin_t;

/**
 * private data of attr_sql plugin
 */
struct private_attr_sql_plugin_t {

	/**
	 * implements plugin interface
	 */
	attr_sql_plugin_t public;

	/**
	 * database connection instance
	 */
	database_t *db;

	/**
	 * configuration attributes
	 */
	sql_attribute_t *attribute;

};

/**
 * Implementation of plugin_t.destroy
 */
static void destroy(private_attr_sql_plugin_t *this)
{
	lib->attributes->remove_provider(lib->attributes, &this->attribute->provider);
	this->attribute->destroy(this->attribute);
	this->db->destroy(this->db);
	free(this);
}

/*
 * see header file
 */
plugin_t *plugin_create()
{
	char *uri;
	private_attr_sql_plugin_t *this;

	uri = lib->settings->get_str(lib->settings, "libstrongswan.plugins.attr-sql.database", NULL);
	if (!uri)
	{
		DBG1("attr-sql plugin: database URI not set");
		return NULL;
	}

	this = malloc_thing(private_attr_sql_plugin_t);

	this->public.plugin.destroy = (void(*)(plugin_t*))destroy;

	this->db = lib->db->create(lib->db, uri);
	if (!this->db)
	{
		DBG1("attr-sql plugin failed to connect to database");
		free(this);
		return NULL;
	}
	this->attribute = sql_attribute_create(this->db);
	lib->attributes->add_provider(lib->attributes, &this->attribute->provider);

	return &this->public.plugin;
}
