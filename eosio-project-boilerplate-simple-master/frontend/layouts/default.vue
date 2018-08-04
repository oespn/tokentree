<template>
  <v-app>
    <v-content>
      <nuxt />
    </v-content>
  </v-app>
</template>

<script>
import Toolbar from '~/components/Toolbar'
export default {
  components: {
    Toolbar
  }
}
</script>

<style lang="stylus">
  @import '~vuetify/src/stylus/main'
</style>
