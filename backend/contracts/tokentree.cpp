#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

//#include "tokentree.hpp"

using namespace eosio;

//namespace tokentree {

class treestory : public eosio::contract {
  public:
    // a treestory begins with a sponsor
    // the sponsor sets the location, description and the offervalue (in TRE), radius in meters
    // contract has a life or maximum quota of trees?

    //** expansion: the treestory would originate from a greater sponsorship offer 
    // the sponsorship offer contains: 
    // - organiser
    // - logistics
    // - prefererred tree type
    // - conditions and warnings 

    // once created a planter can take on the offer for it's value
    // a % of the offer is set aside for validators
    // the validatons record a vote & comment

    // payout checks that at least min_validation count validations have been done 

    treestory(account_name s):
        contract(s), // initialization of the base class for the contract
        _treepatchs(s, s), // initialize the table with code and scope NB! Look up definition of code and scope
        _trees(s,s),
        _validations(s, s)
    {
    }

    // geo_double idx_long_double 
    struct geo_location {
        double latitude;
        double longitude;
        //later: expand to support a region list or lat,lng vectors 
    };

    /// @abi action
    void addoffer(account_name username, double geo, uint32_t radius, const std::string& description, uint64_t offervalue) {
        require_auth(username); // this user is a sponsor
        // Let's make sure the primary key doesn't exist
        // _sponsor.end() is in a way similar to null and it means that the offervalue isn't found
        eosio_assert(_treepatchs.find(geo) == _treepatchs.end(), "This lat,lng already exists in the treestory");
        _treepatchs.emplace(get_self(), [&]( auto& p ) {
            p.sponsor = username;
            p.geo = geo;         
            //p.geo_location.latitude = getSignificant(geo);
            //p.geo_location.longitude = getDecimals(geo);
            p.radius = radius;
            p.description = description;
            p.offervalue = offervalue;

        });
    } 

    /// @abi action
    void plant(account_name username, double geo, double treepatchkey, time planted) {
        require_auth(username); // this user is a planter
        // Let's make sure the primary key doesn't exist
        // _sponsor.end() is in a way similar to null and it means that the offervalue isn't found
        eosio_assert(_trees.find(geo) == _trees.end(), "This lat,lng of tree already exists in the treestory treelist");
        //*** assert hasn't expired 

        // locate the treepatch by treepatchId
        //eosio_assert(_trees.find(geo) == _trees.end(), "This tree patch offer has expired");
        
        //get treepatchId
        auto treepatch = _treepatchs.find(treepatchkey);
        _trees.emplace(get_self(), [&]( auto& p ) {
            p.planter = username;
            p.sponsor = treepatch->sponsor;
            p.geo = geo;         
            //p.geo_location.latitude = getSignificant(geo);
            //p.geo_location.longitude = getDecimals(geo);
            p.planted = planted;
        });
    } 

    /// @abi action
    void validate(account_name username, double geo, time validated, std::string comment, uint8_t health) {
        require_auth(username); // this user is a validator
        // Let's make sure the primary key doesn't exist
        // key = treeId, datetime, user
        auto tree = _trees.find(geo);
        eosio_assert(tree == _trees.end(), "This lat,lng of tree already exists in the treestory treelist");
        //*** assert hasn't expired 

        _validations.emplace(get_self(), [&]( auto& p ) {
            //p.geo = geo;     
            p.treegeokey = geo;
            p.validator = username; // the user who validated primary key
            p.when = validated;
            p.datetimestamp = validated; // as epoch
            p.comment = comment;
            p.health = health;
        });
    } 

/*
    /// @abi action
    void payout(account_name username, time closed) {
        require_auth(username); // this user is a validator


        // Let's make sure the primary key doesn't exist
        // key = treeId, datetime, user
        //auto tree = _trees.find(geo);
         assert 
        eosio_assert(tree == _trees.end(), "This lat,lng of tree already exists in the treestory treelist");

        _validations.emplace(get_self(), [&]( auto& p ) {
            //p.geo = geo;     
            p.treegeokey = geo;
            p.validator = username; // the user who validated primary key
            p.when = validated;
            p.datetimestamp = validated; // as epoch
            p.comment = comment;
            p.health = health;
        });
    } */

  private: 
    // Setup the struct that represents the row in the table


    
    /// @abi table treepatch
    struct treepatch {
      double geo; // primary key, geo location
        //geo_location.latitude = getSignificant(geo);
        //geo_location.longitude = getDecimals(geo);
      uint32_t radius;
      std::string description;
      uint64_t offervalue;

      std::string sponsor; // sponsor primary key
      time offered; // set when user plants the tree

      double primary_key()const { return geo; }  
      // PK is DMS Degree, Minutes, Seconds format to agree with a single long double 
      // See: https://stackoverflow.com/questions/1140189/converting-latitude-and-longitude-to-decimal-values
      uint64_t by_offervalue()const { return offervalue; }

    };
    /// @abi table
    typedef eosio::multi_index< N(treepatch), treepatch, indexed_by<N(byoffervalue), const_mem_fun<treepatch, uint64_t, &treepatch::by_offervalue>>> treepatchs;
    //** expansion: mongodb provide the efficent geo lookup index  
    
    /// @abi table tree
    struct tree {
      double geo; // primary key, geo location
        //geo_location.latitude = getSignificant(geo);
        //geo_location.longitude = getDecimals(geo);
      uint64_t earned;  // this is determined and distributed when the contract ends

      std::string sponsor; // sponsor primary key
      std::string planter; // planter primary key
      time planted; // set when user plants the tree

      uint8_t approxheight; // indication of tree maturity

      double primary_key()const { return geo; }  
      uint64_t by_approxheightvalue()const { return approxheight; }

    };
    /// @abi table
    typedef eosio::multi_index< N(tree), tree, indexed_by<N(by_approxheightvalue), const_mem_fun<tree, uint64_t, &tree::by_approxheightvalue>>> trees;
    //** expansion: mongodb provide the efficent geo lookup index  
    

    struct validation {
        double treegeokey;
        std::string validator; // the user who validated primary key
        time when;
        uint64_t datetimestamp;
        std::string comment;
        uint8_t health = 100;  // default to 100%

        // hash of tree PK + verification
        double primary_key()const { return treegeokey+datetimestamp;  }  
        uint64_t by_treepatchgeokey()const { return treegeokey; }
    };
    /// @abi table
    typedef eosio::multi_index< N(validations), validation, indexed_by<N(treegeokey), const_mem_fun<validation, uint64_t, &validation::by_treepatchgeokey>>> validations;
    
    treepatchs _treepatchs;
    trees _trees;
    validations _validations;  


};

EOSIO_ABI( treestory, (addoffer)(plant)(validate) )
//(payout) 


