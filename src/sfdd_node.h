#ifndef SFDD_NODE
#define SFDD_NODE

namespace sfdd {

class SfddManager;
class SfddElement;

class SfddNode {
public:
    // int constant = 0;  // true: 1, false: -1, non-constant: 0
    // int lit = 0;  // constant:0, non-constant: x/-x
    NodeType type_; // nodetype
    int literal_ = -1;  /* terminal nodes: 0 respects false, 1 respects true; 2*x respects the xth variable (x);
                        2 * x + 1 respects the negation of the xth variable (-x);
                        nonterminal: -1
                    */
    vector<SfddElement> decomp_;
    int vtree_index = 0;  // respect to vtree node
    size_t hash_value = 0;
    unsigned int refcount_ = 0;  // for gc
public:
    SfddNode() { decomp_.clear(); }
    SfddNode(const int v, const int i = 0) {  decomp_.clear(); literal_ = v; vtree_index = i; }
    SfddNode(const SfddNode & s) { literal_ = s.literal_; decomp_ = s.decomp_; vtree_index = s.vtree_index; }
    ~SfddNode() { literal_ = -1; decomp_.clear(); vtree_index = 0; }
    int size() const;
    NodeType type() const { return decomp_.empty() ? NodeType::LIT : NodeType::DECOMP; }
    int literal() const { return literal_; }
    const std::vector<ZsddElement>& decomposition() const { return decomp_; }
    int vtree_node_id() const { return vtree_index; }

    inline bool is_terminal() const { return literal_>-1; }
    inline bool is_positive() const { return literal_>1 && literal_%2==0; }
    inline bool is_negative() const { return literal_>1 && literal_%2==1; }
    inline bool is_empty() const { return !is_terminal() && decomp_.empty(); }
    inline bool is_zero() const { return literal_==0; }
    inline bool is_one() const { return literal_==1; }
    inline bool is_constant() const { return is_zero() || is_one(); }
    // bool equals(const SfddNode& sfdd_node) const;
    bool operator==(const SfddNode& sfdd_node) const;
    SfddNode& operator=(const SfddNode& sfdd_node);
    
    // void print(int indent = 0) const;
    // void print_dot(fstream & out_dot, bool root = false, int depth = 0, string dec_name = "Dec_0_1") const;
    // void save_file_as_dot(const string f_name) const;
};
}  // namespace sfdd

namespace std {
template <> struct hash<sfdd::SfddNode> {
    std::size_t operator()(const sfdd::SfddNode& n) const {
        size_t h = 0;
        if (n.type() == sfdd::NodeType::LIT) {
            sfdd::hash_combine(h, hash<int>()(n.literal()));
            sfdd::hash_combine(h, hash<int>()(n.vtree_node_id()));
            return h;
        } else if (n.type() == sfdd::NodeType::DECOMP) {
            for (const auto& e : n.decomposition()) {
                sfdd::hash_combine(h, hash<sfdd::addr_t>()(e.first));
                sfdd::hash_combine(h, hash<sfdd::addr_t>()(e.second));
            }
            sfdd::hash_combine(h, hash<int>()(n.vtree_node_id()));
        } 
        return h;
    }
};
}  // namespace sfdd

#endif
