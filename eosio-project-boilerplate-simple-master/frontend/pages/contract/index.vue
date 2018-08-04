<template>
  <v-layout justify-center align-center fill-height>
    <v-text-field v-model="lat" placeholder="latitude"/>
    <v-text-field v-model="lng" placeholder="longitude"/>
    <v-btn @click="postOffer">Post Offer</v-btn>
  </v-layout>
</template>




<script>
import eos from "~/lib/eos"


export default {
  data: () => ({
    lat: 0.0,
    lng: 0.0,
  }),
  methods: {
    postOffer(){
        console.log(this.lat, this.lng);
      //postOffer: {
        //lat,lng,radius,desc,tokens    
      //}
    }  
  }
}
</script>

  