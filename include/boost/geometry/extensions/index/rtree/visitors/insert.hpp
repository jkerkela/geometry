// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Boost.Index - R-tree insert details
//
// Copyright 2011 Adam Wulkiewicz.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_VISITORS_INSERT_HPP
#define BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_VISITORS_INSERT_HPP

#include <boost/geometry/extensions/index/algorithms/content.hpp>

#include <boost/geometry/extensions/index/rtree/node/node.hpp>

namespace boost { namespace geometry { namespace index {

namespace detail { namespace rtree { namespace visitors {

namespace detail {

// Default choose_next_node
template <typename Value, typename Options, typename Box, typename Allocators, typename ChooseNextNodeTag>
struct choose_next_node;

template <typename Value, typename Options, typename Box, typename Allocators>
struct choose_next_node<Value, Options, Box, Allocators, choose_by_content_diff_tag>
{
    typedef typename rtree::node<Value, typename Options::parameters_type, Box, Allocators, typename Options::node_tag>::type node;
    typedef typename rtree::internal_node<Value, typename Options::parameters_type, Box, Allocators, typename Options::node_tag>::type internal_node;
    typedef typename rtree::leaf<Value, typename Options::parameters_type, Box, Allocators, typename Options::node_tag>::type leaf;

    typedef typename rtree::elements_type<internal_node>::type children_type;

    typedef typename index::default_content_result<Box>::type content_type;

    template <typename Indexable>
    static inline size_t apply(internal_node & n, Indexable const& indexable, size_t /*node_relative_level*/)
    {
        children_type & children = rtree::elements(n);

        BOOST_GEOMETRY_INDEX_ASSERT(!children.empty(), "can't choose the next node if children are empty");

        size_t children_count = children.size();

        // choose index with smallest content change or smallest content
        size_t choosen_index = 0;
        content_type smallest_content_diff = std::numeric_limits<content_type>::max();
        content_type smallest_content = std::numeric_limits<content_type>::max();

        // caculate areas and areas of all nodes' boxes
        for ( size_t i = 0 ; i < children_count ; ++i )
        {
            typedef typename children_type::value_type child_type;
            child_type const& ch_i = children[i];

            // expanded child node's box
            Box box_exp(ch_i.first);
            geometry::expand(box_exp, indexable);

            // areas difference
            content_type content = index::content(box_exp);
            content_type content_diff = content - index::content(ch_i.first);

            // update the result
            if ( content_diff < smallest_content_diff ||
                ( content_diff == smallest_content_diff && content < smallest_content ) )
            {
                smallest_content_diff = content_diff;
                smallest_content = content;
                choosen_index = i;
            }
        }

        return choosen_index;
    }
};

// ----------------------------------------------------------------------- //

// Not implemented here
template <typename Value, typename Options, typename Translator, typename Box, typename Allocators, typename RedistributeTag>
struct redistribute_elements
{
    BOOST_MPL_ASSERT_MSG(
        (false),
        NOT_IMPLEMENTED_FOR_THIS_REDISTRIBUTE_TAG_TYPE,
        (redistribute_elements));
};

// ----------------------------------------------------------------------- //

// Split algorithm
template <typename Value, typename Options, typename Translator, typename Box, typename Allocators, typename SplitTag>
class split
{
    BOOST_MPL_ASSERT_MSG(
        (false),
        NOT_IMPLEMENTED_FOR_THIS_SPLIT_TAG_TYPE,
        (split));
};

// Default split algorithm
template <typename Value, typename Options, typename Translator, typename Box, typename Allocators>
class split<Value, Options, Translator, Box, Allocators, split_default_tag>
{
protected:
    typedef typename rtree::node<Value, typename Options::parameters_type, Box, Allocators, typename Options::node_tag>::type node;
    typedef typename rtree::internal_node<Value, typename Options::parameters_type, Box, Allocators, typename Options::node_tag>::type internal_node;
    typedef typename rtree::leaf<Value, typename Options::parameters_type, Box, Allocators, typename Options::node_tag>::type leaf;

    typedef typename Options::parameters_type parameters_type;

public:
    template <typename Node>
    static inline void apply(node* & root_node,
                             size_t & leafs_level,
                             Node & n,
                             internal_node *parent_node,
                             size_t current_child_index,
                             Translator const& tr,
                             Allocators & allocators)
    {
        // create additional node
        node * second_node = rtree::create_node<Allocators, Node>::apply(allocators);
        Node & n2 = rtree::get<Node>(*second_node);

        // redistribute elements
        Box box1, box2;
        redistribute_elements<Value, Options, Translator, Box, Allocators, typename Options::redistribute_tag>::
            apply(n, n2, box1, box2, tr);

        // check numbers of elements
        BOOST_GEOMETRY_INDEX_ASSERT(parameters_type::min_elements <= rtree::elements(n).size() &&
            rtree::elements(n).size() <= parameters_type::max_elements,
            "unexpected number of elements");
        BOOST_GEOMETRY_INDEX_ASSERT(parameters_type::min_elements <= rtree::elements(n2).size() &&
            rtree::elements(n2).size() <= parameters_type::max_elements,
            "unexpected number of elements");

        // node is not the root - just add the new node
        if ( parent_node != 0 )
        {
            // update old node's box
            rtree::elements(*parent_node)[current_child_index].first = box1;
            // add new node to the parent's children
            rtree::elements(*parent_node).push_back(std::make_pair(box2, second_node));
        }
        // node is the root - add level
        else
        {
            BOOST_GEOMETRY_INDEX_ASSERT(&n == rtree::get<Node>(root_node), "node should be the root");

            // create new root and add nodes
            node * new_root = rtree::create_node<Allocators, internal_node>::apply(allocators);

            rtree::elements(rtree::get<internal_node>(*new_root)).push_back(std::make_pair(box1, root_node));
            rtree::elements(rtree::get<internal_node>(*new_root)).push_back(std::make_pair(box2, second_node));

            root_node = new_root;
            ++leafs_level;
        }
    }
};

// ----------------------------------------------------------------------- //

// Default insert visitor
template <typename Element, typename Value, typename Options, typename Translator, typename Box, typename Allocators>
class insert
    : public rtree::visitor<Value, typename Options::parameters_type, Box, Allocators, typename Options::node_tag, false>::type
    , index::nonassignable
{
protected:
    typedef typename rtree::node<Value, typename Options::parameters_type, Box, Allocators, typename Options::node_tag>::type node;
    typedef typename rtree::internal_node<Value, typename Options::parameters_type, Box, Allocators, typename Options::node_tag>::type internal_node;
    typedef typename rtree::leaf<Value, typename Options::parameters_type, Box, Allocators, typename Options::node_tag>::type leaf;

    typedef typename Options::parameters_type parameters_type;

    inline insert(node* & root,
                  size_t & leafs_level,
                  Element const& element,
                  Translator const& t,
                  Allocators & allocators,
                  size_t relative_level = 0
    )
        : m_element(element)
        , m_tr(t)
        , m_relative_level(relative_level)
        , m_level(leafs_level - relative_level)
        , m_root_node(root)
        , m_leafs_level(leafs_level)
        , m_parent(0)
        , m_current_child_index(0)
        , m_current_level(0)
        , m_allocators(allocators)
    {
        BOOST_GEOMETRY_INDEX_ASSERT(m_relative_level <= leafs_level, "unexpected level value");
        BOOST_GEOMETRY_INDEX_ASSERT(m_level <= m_leafs_level, "unexpected level value");
        BOOST_GEOMETRY_INDEX_ASSERT(0 != m_root_node, "there is no root node");
        // TODO
        // assert - check if Box is correct
    }

    template <typename Visitor>
    inline void traverse(Visitor & visitor, internal_node & n)
    {
        // choose next node
        size_t choosen_node_index = detail::choose_next_node<Value, Options, Box, Allocators, typename Options::choose_next_node_tag>::
            apply(n, rtree::element_indexable(m_element, m_tr), m_leafs_level - m_current_level);

        // expand the node to contain value
        geometry::expand(
            rtree::elements(n)[choosen_node_index].first,
            rtree::element_indexable(m_element, m_tr));

        // next traversing step
        traverse_apply_visitor(visitor, n, choosen_node_index);
    }

    // TODO: awulkiew - change post_traverse name to handle_overflow or overflow_treatment?

    template <typename Node>
    inline void post_traverse(Node &n)
    {
        BOOST_GEOMETRY_INDEX_ASSERT(0 == m_parent || &n == rtree::get<Node>(rtree::elements(*m_parent)[m_current_child_index].second),
                                    "if node isn't the root current_child_index should be valid");

        // handle overflow
        if ( parameters_type::max_elements < rtree::elements(n).size() )
        {
            split(n);
        }
    }

    template <typename Visitor>
    inline void traverse_apply_visitor(Visitor & visitor, internal_node &n, size_t choosen_node_index)
    {
        // save previous traverse inputs and set new ones
        internal_node * parent_bckup = m_parent;
        size_t current_child_index_bckup = m_current_child_index;
        size_t current_level_bckup = m_current_level;

        // calculate new traverse inputs
        m_parent = &n;
        m_current_child_index = choosen_node_index;
        ++m_current_level;

        // next traversing step
        rtree::apply_visitor(visitor, *rtree::elements(n)[choosen_node_index].second);

        // restore previous traverse inputs
        m_parent = parent_bckup;
        m_current_child_index = current_child_index_bckup;
        m_current_level = current_level_bckup;
    }

    template <typename Node>
    inline void split(Node & n) const
    {
        detail::split<Value, Options, Translator, Box, Allocators, typename Options::split_tag>::apply(m_root_node, m_leafs_level, n, m_parent, m_current_child_index, m_tr, m_allocators);
    }

    // TODO: awulkiew - implement dispatchable split::apply to enable additional nodes creation

    Element const& m_element;
    Translator const& m_tr;
    const size_t m_relative_level;
    const size_t m_level;

    node* & m_root_node;
    size_t & m_leafs_level;

    // traversing input parameters
    internal_node *m_parent;
    size_t m_current_child_index;
    size_t m_current_level;

    Allocators & m_allocators;
};

} // namespace detail

// Insert visitor forward declaration
template <typename Element, typename Value, typename Options, typename Translator, typename Box, typename Allocators, typename InsertTag>
struct insert;

// Default insert visitor used for nodes elements
template <typename Element, typename Value, typename Options, typename Translator, typename Box, typename Allocators>
struct insert<Element, Value, Options, Translator, Box, Allocators, insert_default_tag>
    : public detail::insert<Element, Value, Options, Translator, Box, Allocators>
{
    typedef detail::insert<Element, Value, Options, Translator, Box, Allocators> base;
    typedef typename base::node node;
    typedef typename base::internal_node internal_node;
    typedef typename base::leaf leaf;

    inline insert(node* & root,
                  size_t & leafs_level,
                  Element const& element,
                  Translator const& tr,
                  Allocators & allocators,
                  size_t relative_level = 0
    )
        : base(root, leafs_level, element, tr, allocators, relative_level)
    {}

    inline void operator()(internal_node & n)
    {
        BOOST_GEOMETRY_INDEX_ASSERT(base::m_current_level < base::m_leafs_level, "unexpected level");

        if ( base::m_current_level < base::m_level )
        {
            // next traversing step
            base::traverse(*this, n);
        }
        else
        {
            BOOST_GEOMETRY_INDEX_ASSERT(base::m_level == base::m_current_level, "unexpected level");

            // push new child node
            rtree::elements(n).push_back(base::m_element);
        }

        base::post_traverse(n);
    }

    inline void operator()(leaf &)
    {
        assert(false);
    }
};

// Default insert visitor specialized for Values elements
template <typename Value, typename Options, typename Translator, typename Box, typename Allocators>
struct insert<Value, Value, Options, Translator, Box, Allocators, insert_default_tag>
    : public detail::insert<Value, Value, Options, Translator, Box, Allocators>
{
    typedef detail::insert<Value, Value, Options, Translator, Box, Allocators> base;
    typedef typename base::node node;
    typedef typename base::internal_node internal_node;
    typedef typename base::leaf leaf;

    inline insert(node* & root,
                  size_t & leafs_level,
                  Value const& v,
                  Translator const& t,
                  Allocators & allocators,
                  size_t relative_level = 0
    )
        : base(root, leafs_level, v, t, allocators, relative_level)
    {}

    inline void operator()(internal_node & n)
    {
        BOOST_GEOMETRY_INDEX_ASSERT(base::m_current_level < base::m_leafs_level, "unexpected level");
        BOOST_GEOMETRY_INDEX_ASSERT(base::m_current_level < base::m_level, "unexpected level");

        // next traversing step
        base::traverse(*this, n);

        base::post_traverse(n);
    }

    inline void operator()(leaf & n)
    {
        BOOST_GEOMETRY_INDEX_ASSERT(base::m_current_level == base::m_leafs_level, "unexpected level");
        BOOST_GEOMETRY_INDEX_ASSERT(base::m_level == base::m_current_level || base::m_level == std::numeric_limits<size_t>::max(), "unexpected level");
        
        rtree::elements(n).push_back(base::m_element);

        base::post_traverse(n);
    }
};

}}} // namespace detail::rtree::visitors

}}} // namespace boost::geometry::index

#endif // BOOST_GEOMETRY_EXTENSIONS_INDEX_RTREE_VISITORS_INSERT_HPP
