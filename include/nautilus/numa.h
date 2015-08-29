#ifndef __NUMA_H__
#define __NUMA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <nautilus/list.h>

#define MAX_NUMA_DOMAINS 128

struct numa_domain {
    uint32_t id;
    uint64_t addr_space_size;
    uint32_t num_regions;

    struct list_head regions;

    /* list of other domains, ordered by distance */
    struct list_head adj_list;
};

struct buddy_mempool;

struct mem_reg_entry {
    struct mem_region * mem;
    struct list_head mem_ent;
};

/* for adjacency lists */
struct domain_adj_entry {
    struct numa_domain * domain;
    struct list_head list_ent;
};


struct mem_region {
    uint32_t domain_id;
    uint64_t base_addr;
    uint64_t len;
    uint8_t  enabled;
    uint8_t  hot_pluggable;
    uint8_t  nonvolatile;

    /* used by the kernel memory allocator */
    struct buddy_mempool * mm_state;

    struct list_head entry;

    struct list_head glob_link;
};

struct nk_locality_info {
    uint32_t  num_domains;
    uint8_t * numa_matrix;

    struct numa_domain * domains[MAX_NUMA_DOMAINS];
};

struct cpu;
int nk_cpu_topo_discover(struct cpu* me);
int nk_numa_init(void);
void nk_dump_numa_info(void);
unsigned nk_my_numa_node(void);
unsigned nk_get_num_domains(void);
struct mem_region * nk_get_base_region_by_cpu(unsigned cpu);
struct mem_region * nk_get_base_region_by_num (unsigned num);


struct nk_topo_params {
    uint32_t smt_bits;
    uint32_t core_bits;
};

struct nk_cpu_coords {
    uint32_t smt_id;  // logical CPU ID within core
    uint32_t core_id; // physical core within package
    uint32_t pkg_id;  // package id
};





#ifdef __cplusplus
}
#endif

#endif
