<template>
  <v-layout justify-center align-center fill-height v-if="!location">
    <v-btn @click="getPosition">Please enable geolocation</v-btn>
  </v-layout>
  <v-layout fill-height column v-else>
    <Map v-if="location" :trees="trees" :center="location"></Map>
    <v-bottom-nav :value="true">
      <Verifications v-model="verify" :trees="trees"></Verifications>
      <v-btn @click="plant = true" flat>
        <span>Plant</span>
        <v-icon>nature</v-icon>
      </v-btn>
      <v-btn @click="verify = true" flat>
        <span>Verify</span>
        <v-icon>nature_people</v-icon>
      </v-btn>
    </v-bottom-nav>
  </v-layout>
</template>

<script>
import { mapGetters, mapMutations } from 'vuex'
import Map from '~/components/Map'
import Verifications from '~/components/Verifications'
export default {
  components: {
    Map,
    Verifications
  },
  data: () => ({
    verify: false,
    plant: false,
  }),
  computed: mapGetters({
    trees: 'trees',
    location: 'location',
  }),
  methods: {
    ...mapMutations({
      setLocation: 'setLocation'
    }),
    getPosition () {
      navigator.geolocation
        .getCurrentPosition(x => this.setLocation(x.coords))
    }
  },
  mounted () {
    this.getPosition()
  }
}
</script>
