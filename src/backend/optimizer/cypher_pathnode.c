#include "postgres.h"

#include "nodes/extensible.h"
#include "nodes/nodes.h"
#include "nodes/pg_list.h"
#include "nodes/relation.h"

#include "optimizer/cypher_createplan.h"
#include "optimizer/cypher_pathnode.h"

const CustomPathMethods cypher_create_path_methods = {
    "Cypher Create", plan_cypher_create_path, NULL};

CustomPath *create_cypher_create_path(PlannerInfo *root, RelOptInfo *rel,
                                      List *custom_private)
{
    CustomPath *cp;

    cp = makeNode(CustomPath);

    cp->path.pathtype = T_CustomScan;

    cp->path.parent = rel;
    cp->path.pathtarget = rel->reltarget;

    cp->path.param_info = NULL;

    // Do not allow parallel methods
    cp->path.parallel_aware = false;
    cp->path.parallel_safe = false;
    cp->path.parallel_workers = 0;

    cp->path.rows = 0; // Basic CREATE will not return rows
    cp->path.startup_cost = 0; // Basic CREATE will not fetch any pages
    cp->path.total_cost = 0;

    // No output ordering for basic CREATE
    cp->path.pathkeys = NULL;

    // Disable all custom flags for now
    cp->flags = 0;

    cp->custom_paths = NIL; // Basic CREATE does not have children
    cp->custom_private = custom_private;
    cp->methods = &cypher_create_path_methods;

    return cp;
}