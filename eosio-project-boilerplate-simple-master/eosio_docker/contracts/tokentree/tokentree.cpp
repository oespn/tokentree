#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
using namespace eosio;

class treestory : public eosio::contract {
  public:
    // a treestory begins with a sponsor
    // the sponsor sets the location, description and the offervalue

    //** expansion: the treestory would originate from a greater sponsorship offer 
    // the sponsorship offer contains: 
    // - organiser
    // - logistics
    // - prefererred tree type
    // - conditions and warnings 

    // once created a planter can take on the offer for it's value
    // a % of the offer is set aside for validators
    // the validators have a maximum number
    // the validatons record a vote 

    treestory(account_name s):
        contract(s), // initialization of the base class for the contract
        _trees(s, s) // initialize the table with code and scope NB! Look up definition of code and scope
    {
    }

    // geo_double idx_long_double 
    struct geo_location {
        double latitude;
        double longitude;
        //later: expand to support a region list or lat,lng vectors 
    };

    /// @abi action
    void create(account_name username, long double geo, const std::string& description, uint64_t offervalue) {
        require_auth(username);
        // Let's make sure the primary key doesn't exist
        // _sponsor.end() is in a way similar to null and it means that the offervalue isn't found
        eosio_assert(_trees.find(geo) == _trees.end(), "This lat,lng already exists in the treestory");
        _trees.emplace(get_self(), [&]( auto& p ) {
            p.geo = geo;
            //p.geo_location.latitude = getSignificant(geo);
            //p.geo_location.longitude = getDecimals(geo);
            p.description = description;
            p.offervalue = offervalue;
        });
    } 

  private: 
    // Setup the struct that represents the row in the table
    /// @abi table sponsor
    struct tree {
      long double geo = geo; // primary key, geo location
        //geo_location.latitude = getSignificant(geo);
        //geo_location.longitude = getDecimals(geo);

      std::string description;
      uint64_t offervalue;

      long double primary_key()const { return geo; }
      uint64_t by_offervalue()const { return offervalue; }
    };


    // We setup the table:
    /// @abi table
    typedef eosio::multi_index< N(trees), tree, indexed_by<N(byoffervalue), const_mem_fun<tree, uint64_t, &tree::by_offervalue>>> trees;
    //** expansion: mongodb provide the efficent geo lookup index  

    trees _trees;

};

 EOSIO_ABI( treestory, (create) )