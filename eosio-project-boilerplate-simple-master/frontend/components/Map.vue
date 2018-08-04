<template>
  <GmapMap
    :center="center"
    :zoom="10">
    <template v-for="tree in trees">
      <GmapMarker :key="tree.id"
        :position="{ lat: tree.lat, lng: tree.lng }"
        clickable
        @click="tree.selected = true">
      </GmapMarker>
      <GmapInfoWindow :key="tree.id"
        v-if="tree.selected"
        :position="{ lat: tree.lat, lng: tree.lng }"
        @closeclick="tree.selected = false">
        hello
      </GmapInfoWindow>
    </template>
  </GmapMap>
</template>

<script>
export default {
  props: {
    trees: {
      type: Array,
      required: true
    },
    center: {
      type: Object,
      required: true
    }
  }
}
</script>

<style>
.vue-map-container {
  height: 100vh;
  height: 100%;
  width: 100vw;
  width: 100%;
}
</style>